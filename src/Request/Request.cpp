# include "../../includes/Request.hpp"
#include <cstring>

Request::Request():_status(0),_headersParsed(false),_bodyParsed(false),_requestLineParsed(false), _bodySize(0) {

}

Request::~Request(){

}

std::string toLowercase(std::string str) {
    std::string newStr(str);
    for (size_t i = 0; i < str.length(); ++i)
        newStr[i] = std::tolower(newStr[i]);
    return (newStr);
}

bool caseInsensitiveStringCompare(const std::string& str1, const std::string& str2) {
    return toLowercase(str1) == toLowercase(str2);
}

// std::string Request::getBody() const{
//     return _body;
// }

void Request::collectData(){
    int         lineNumber = 0;
    char        *token;

    token = strtok((char *)_headers.c_str(),"\r\n");
    while (token)
    {
        std::string stoken(token);
        lineNumber++;
        switch (lineNumber)
        {
            case 1 :
                RequestLineParser(stoken);
                break;
            default:
                collector(stoken);
                break;
        }
        token = strtok(NULL , "\r\n");
    }
}

void Request::collector(std::string &token){
    std::size_t separatorPos;

    separatorPos = token.find(": ");
    if(separatorPos != std::string::npos){
        _requestInfos.insert(std::pair<std::string , std::string>(toLowercase(token.substr(0, separatorPos)), toLowercase(token.substr(separatorPos + 2))));
    }
}

void Request::requestParser(const char *request ,std::vector<Location> &locations, size_t readBytes){
    try{
        splitingHeaderBody(request, readBytes);
        if(_headersParsed) {
            if (_headersParsed && _requestLineParsed) {
                pathInCannonicalForm();
                matchingLocation(locations);
                isallowedMethod();
                checkingBadRequests();
            }
        }
        for (std::map<std::string, std::string>::iterator it = _requestInfos.begin(); it != _requestInfos.end(); ++it)
            std::cout << it->first << " => " << it->second << std::endl;
        bodyHandler();
    } catch ( ClientError &e) {
        _status = e;
        std::cerr << "Error: " << e << std::endl;
    }
}

std::map<std::string , std::string> Request::getRequestInfo() const{
    return _requestInfos;
}

void Request::splitingHeaderBody(const char *request, size_t readBytes){
    size_t it;
    _body.clear();
    if(_headersParsed){
        readingBody(request, readBytes);
    }
    else {
        if ((it = std::string(request).find("\r\n\r\n")) != std::string::npos){
            _headers = std::string(request).substr(0, it);
            collectData();
            _headersParsed = true;
            if(readBytes - it - 4 <= 0)
                _bodyParsed = true;
            readingBody(request + it + 4, readBytes - it - 4);
        }
        else {
            _headers = std::string(request);
            collectData();
        }
    }
}

void Request::RequestLineParser(const std::string& requestLine) {
    if(_requestLineParsed)
        return;
    std::vector<std::string> methods;
    methods.push_back("GET");
    methods.push_back("POST");
    methods.push_back("DELETE");

    std::vector<std::string> versions;
    versions.push_back("HTTP/1.0");
    versions.push_back("HTTP/1.1");
    versions.push_back("HTTP/2.0");

    int numSpaces = std::count(requestLine.begin(), requestLine.end(), ' ');

    std::istringstream iss(requestLine.c_str());
    std::vector<std::string> parts;
    std::string part;
    while (std::getline(iss, part, ' ')) {
        parts.push_back(part);
    }
    if (numSpaces != 2 || parts.size() != 3)
        throw BADREQUEST;

    bool startsWithMethod = false;
    for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it) {
        if (parts[0] == *it) {
            startsWithMethod = true;
            _requestInfos.insert(std::make_pair("method", toLowercase(*it)));
            break;
        }
    }
    if (!startsWithMethod)
        throw BADREQUEST;
    bool endsWithVersion = false;
    for (std::vector<std::string>::const_iterator it = versions.begin(); it != versions.end(); ++it) {
        if (parts[2] == *it) {
            endsWithVersion = true;
            _requestInfos.insert(std::make_pair("version", toLowercase(*it)));
            break;
        }
    }
    if (!endsWithVersion)
        throw BADREQUEST;
    _requestInfos.insert(std::make_pair("path", parts[1]));
    isValidUri(parts[1]);
    _requestLineParsed = true;
}

void isValidUri(std::string uri){
    if(uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$'()*+,;=%") != std::string::npos)
        throw BADREQUEST;
}

void Request::checkingBadRequests(){
    if (_headersParsed == true)
    {
        std::map<std::string, std::string>::iterator it = _requestInfos.find("transfer-encoding");
        if(it != _requestInfos.end() && it->second.compare("chunked"))
            throw BADREQUEST;
        if(_requestInfos["version"].compare("HTTP/1.1") && _requestInfos.find("host") == _requestInfos.end())
            throw BADREQUEST;
        if (_requestInfos.find("transfer-encoding") == _requestInfos.end() && _requestInfos.find("content-length") ==_requestInfos.end() && !_requestInfos["method"].compare("post"))
            throw BADREQUEST;
        if(!_requestInfos["method"].compare("post") && 
            _requestInfos.find("transfer-encoding") == _requestInfos.end() &&
            _requestInfos.find("content-length") == _requestInfos.end())
                throw BADREQUEST;
        if(_requestInfos["path"].length() >= 2048)
            throw REQUESTURITOOLONG;
    }
}

void Request::pathInCannonicalForm(){
    std::string                 path;
    char                        *token;

    token = strtok((char *)this->_requestInfos["path"].c_str(),"/");
    while (token){
        std::string stoken(token);

        if(stoken== ".") {
            // do nothing 
        } else if(stoken== "..") {
            if(_uriParts.size() > 1){
                _uriParts.pop_back();
            }
        } else {
            _uriParts.push_back(stoken);
        }
        token = strtok(NULL, "/");
    }
    if(_uriParts.size() == 0)
        _uriParts.push_back("");
    for(auto p : _uriParts){
        path.append("/");
        path.append(p);
        std::cout << p << std::endl;
    }
    this->_requestInfos["path"] = path;
}

void Request::matchingLocation(std::vector<Location> &locations){
    std::vector<Location>::iterator   lIt;
    bool                              found = false;

    std::cout << "Matching location" << std::endl;
    std::string upper;

    for(std::vector<Location>::iterator it = locations.begin();it != locations.end();it++){
        std::string pattern = (*it).name;
        if (_requestInfos["path"].length() <  pattern.length())
            continue ;
        std::string lower = _requestInfos["path"].substr(0, pattern.length());
        if (pattern == "/" || (pattern == lower && (_requestInfos["path"][pattern.length()] == '\0' || _requestInfos["path"][pattern.length()] == '/'))) {
            if (upper.empty()) {
                upper = lower;
                this->_location = *it;
            }
            else {
                if (lower.length() > upper.length()) {
                    upper = lower;
                    this->_location = *it;
                }
            }
        }
    }
    if (upper.empty())
        this->_status = NOTFOUND;
}

void Request::isallowedMethod(){
    if (_location.allowMethods.empty()) {
        return;
    }
    for (std::vector<std::string>::const_iterator it = _location.allowMethods.begin(); it != _location.allowMethods.end(); ++it) {
        if (caseInsensitiveStringCompare(_requestInfos["method"], *it)) {
            return;
        }
    }
    throw METHODNOTALLOWED;
}

std::string randomFileGenerator() {
    std::string randomFile;
    std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    srand(time(NULL));
    for (size_t i = 0; i < 10; i++) {
        randomFile += charset[rand() % charset.length()];
    }
    return (randomFile);
}


void Request::bodyHandler(){

    if(_requestInfos["method"].compare("post"))
        return;
    if(_file.empty())
        _file = randomFileGenerator() + getExtension(_requestInfos["content-type"]);
    std::string path = "/home/yassinelr/Desktop/WebServ"+_location.upload_store + "/" + _file;
    std::cout << _location.upload_store << std::endl;
    std::cout << "Path : " << path << std::endl;
    std::ofstream ofs(path, std::ios_base::app | std::ios::binary);
    if (ofs.is_open()) {
        ofs.write(_body.data(), _body.size());
        ofs.close();
        std::cout << _requestInfos["content-length"] << "  "<< this->_bodySize << std::endl;
        std::cout << "File uploaded successfully" << std::endl;
    } else {
        std::cout << "Error opening file" << std::endl;
        perror("Error");
    }
    std::cout <<"Here is the Body :" <<_body.data() << std::endl;
}

void Request::readingBody(const char *body, size_t readBytes){
    if(_requestInfos.find("content-length") != _requestInfos.end()){
        if(_bodySize == 0){
            _bodySize = std::stoi(_requestInfos["content-length"]);
        }
        if(_body.size() + readBytes >= _bodySize){
            _body.insert(_body.end(), body, body + _bodySize - _body.size());
            _bodyParsed = true;
        }
        else {
            _body.insert(_body.end(), body, body + readBytes);
        }
    }
    // else if(_requestInfos.find("transfer-encoding") != _requestInfos.end()){
    //     setChunkedBody(body, readBytes);
    // }
    else {
        _bodyParsed = true;
    }
}

// void Request::setChunkedBody(const char *body, size_t readBytes){
//     std::string chunkedBody(body, readBytes);
//     size_t pos = 0;
//     size_t chunkSize;
//     size_t chunkStart;
//     size_t chunkEnd;
//     char   *line;
    // line = strtok(body, "\r\n");
    // while (line) {
        // switch (_chunckState)
        // {
        //     case false:
        //         chunkSize = isChunkSize(line);
        //         break;
        //     case true:
        //         readChunk(line, chunkSize);
        //     default:
        //         break;
        // }
        // body = strtok(NULL , "\r\n");
    // }
// }

// size_t Request::isChunkSize(char *line){
//     size_t chunkSize;
//     std::string chunkSizeStr(line);
//     chunkSize = std::stoul(chunkSizeStr, 0, 16);
//     if(chunkSize == 0){
//         _bodyParsed = true;
//     }
//     _chunckState = true;
//     return chunkSize;
// }

// void Request::readChunk(char *line, size_t chunkSize){
//     // if(_body.size() + strlen(line) >= chunkSize){
//     //     _body.insert(_body.end(), line, line + chunkSize - _body.size());
//     //     _bodyParsed = true;
//     //     _chunckState = false;
//     // }
//     // else {
//         _body.insert(_body.end(), line, line + strlen(line));
//         _chunckState = false;
//     // }
// }