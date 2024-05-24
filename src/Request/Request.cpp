# include "../../includes/Request.hpp"

Request::Request():_status(0),_headersParsed(false),_bodyParsed(false),_requestLineParsed(false), _bodySize(0), _chunckState(false), _checkingRequestInfo(false), _chunkSize(0), _chunkCRLF(false){
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

void Request::extractingQuerryString(){
    size_t  it;

    it = _requestInfos["path"].find("?");
    if(it != std::string::npos){
        _requestInfos["path"] = _requestInfos["path"].substr(0, it);
        _requestInfos.insert(std::pair<std::string , std::string>("query", _requestInfos["path"].substr(it + 1)));
    }
}

void Request::collector(std::string &token){
    std::size_t separatorPos;

    separatorPos = token.find(": ");
    if(separatorPos != std::string::npos){
        _requestInfos.insert(std::pair<std::string , std::string>(toLowercase(token.substr(0, separatorPos)), toLowercase(token.substr(separatorPos + 2))));
    }
}

void Request::requestParser(const char *request ,std::vector<Location> &locations, size_t readBytes , std::string root, std::vector<std::string> index){
    try{
        splitingHeaderBody(request, readBytes, root);
        if(_headersParsed) {
            if (_headersParsed && _requestLineParsed && !_checkingRequestInfo) {
                _index = index;
                pathInCannonicalForm();
                matchingLocation(locations);
                isallowedMethod();
                checkingBadRequests();
                std::cout << "HERE IS THE PATH : " << _requestInfos["path"] << std::endl;
            }
        }
        bodyHandler();
    } catch ( ClientError &e) {
        _status = e;
        std::cerr << "Error: " << e << std::endl;
    }
}

std::map<std::string , std::string> Request::getRequestInfo() const{
    return _requestInfos;
}

void Request::splitingHeaderBody(const char *request, size_t readBytes, std::string rootPath){
    size_t it;

    _body.clear();
    if(_headersParsed){
        readingBody(request, readBytes);
    }
    else {
        if ((it = std::string(request).find("\r\n\r\n")) != std::string::npos){
            _headers = std::string(request).substr(0, it);
            collectData();
            _rootPath = rootPath;
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
            throw NOTIMPLEMENTED;
        if(_requestInfos["version"].compare("HTTP/1.1") && _requestInfos.find("host") == _requestInfos.end())
            throw BADREQUEST;
        if(!_requestInfos["method"].compare("post") && 
            _requestInfos.find("transfer-encoding") == _requestInfos.end() &&
            _requestInfos.find("content-length") == _requestInfos.end())
                throw BADREQUEST;
        if(_requestInfos["path"].length() >= 2048)
            throw REQUESTURITOOLONG;
        _checkingRequestInfo = true;
    }
}

void Request::pathInCannonicalForm(){
    std::string                 path;
    char                        *token;
    bool                        finishWithSlash = false;

    extractingQuerryString();
    if (_requestInfos["path"][_requestInfos["path"].length() - 1] == '/')
        finishWithSlash = true;
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
    for(size_t i = 0; i < _uriParts.size(); i++){
        if(i == 0)
            path = _uriParts[i];
        else
            path += "/" + _uriParts[i];
    }
    this->_requestInfos["path"] = _rootPath + path;
    if (finishWithSlash && _uriParts.size() > 0)
        this->_requestInfos["path"] += "/";
    std::cout << "Path in cannonical form: " << this->_requestInfos["path"] << std::endl;
}

void Request::matchingLocation(std::vector<Location> &locations){
    std::vector<Location>::iterator     lIt;
    bool                                found = false;
    std::string                         upper;

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

    if(_requestInfos["method"].compare("post") || !_location.upload_enable){
        _bodyParsed = true;
        return;
    }
    if(_file.empty())
        _file = randomFileGenerator() + getExtension(_requestInfos["content-type"]);
    std::string path = "/home/yassinelr/Desktop/WebServ"+_location.upload_store + "/" + _file;
    std::ofstream ofs(path, std::ios_base::app | std::ios::binary);
    if (ofs.is_open()) {
        ofs.write(_body.data(), _body.size());
        ofs.close();
        std::cout << "File uploaded successfully" << std::endl;
    } else {
        std::cout << "Error opening file" << std::endl;
        perror("Error");
    }
}

void Request::postChecker(){
    if(_requestInfos["method"].compare("post"))
        return;
    if(_requestInfos.find("content-type") == _requestInfos.end())
        throw BADREQUEST;
}

void Request::readingBody(const char *body, size_t readBytes){
    if(_requestInfos.find("transfer-encoding") != _requestInfos.end()){
        setChunkedBody(body, readBytes);
    }
    else if(_requestInfos.find("content-length") != _requestInfos.end()){
        if(_bodySize + readBytes >= stoi(_requestInfos["content-length"])){
            _body.insert(_body.end(), body, body + stoi(_requestInfos["content-length"]) - _bodySize);
            _bodySize = stoi(_requestInfos["content-length"]);
            _bodyParsed = true;
        }
        else {
            _body.insert(_body.end(), body, body + readBytes);
            _bodySize += readBytes;
        }
    }
    else {
        _bodyParsed = true;
    }
}

void Request::setChunkedBody(const char *body, size_t readBytes) {
    std::string strBody(body, readBytes);
    size_t pos = 0;

    while (true) {
        if (_chunckState == false) {
            if (!_partialChunkSize.empty()) {
                strBody = _partialChunkSize + strBody;
                _partialChunkSize.clear();
            }
            if (_chunkCRLF && strBody.size() >= 2) {
                strBody = strBody.substr(2);
                _chunkCRLF = false;
            }
            pos = strBody.find("\r\n");
            if (pos == std::string::npos) {
                _partialChunkSize.append(strBody);
                return;
            }
            std::string chunkSizeStr = strBody.substr(0, pos);
            try {
                _chunkSize = std::stoul(chunkSizeStr, NULL, 16);
            } catch (const std::exception &e) {
                std::cerr << "Invalid chunk size: " << chunkSizeStr << " Error: " << e.what() << std::endl;
                return;
            }
            strBody = strBody.substr(pos + 2);
            _chunckState = true;
            if (_chunkSize == 0) {
                _bodyParsed = true;
                return;
            }
        }

        if (_chunckState == true) {
            if (strBody.size() >= _chunkSize) {
                _body.insert(_body.end(), strBody.begin(), strBody.begin() + _chunkSize);
                strBody = strBody.substr(_chunkSize);
                _chunkSize = 0;
                _chunkCRLF = true;
                _chunckState = false;
                if (strBody.size() >= 2 && strBody.substr(0, 2) == "\r\n") {
                    strBody = strBody.substr(2);
                    _chunkCRLF = false;
                }
            } else {
                _body.insert(_body.end(), strBody.begin(), strBody.end());
                _chunkSize -= strBody.size();
                return;
            }
        }
    }
}