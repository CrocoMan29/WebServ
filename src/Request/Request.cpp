# include "../../includes/Request.hpp"
#include <cstring>

Request::Request():_status(0),_read(0),_headersParsed(false),_bodyParsed(false),_requestLineParsed(false){

}

Request::~Request(){

}

std::string toLowercase(std::string str) {
    std::string newStr(str);
    for (size_t i = 0; i < str.length(); ++i) {
        newStr[i] = std::tolower(newStr[i]);
    }
    return (newStr);
}

bool caseInsensitiveStringCompare(const std::string& str1, const std::string& str2) {
    std::string lowercaseStr1 = toLowercase(str1);
    std::string lowercaseStr2 = toLowercase(str2);
    // for (std::string::const_iterator it = str2.begin(); it != str2.end(); ++it) {
    //     lowercaseStr2 += tolower(*it);
    // }
    return lowercaseStr1 == lowercaseStr2;
}

std::string Request::getBody() const{
    return _body;
}

std::string Request::getExtension(){
    size_t pos =  _requestInfos["path"].find_last_of(".");
    if (pos == std::string::npos)
        return "";
    return _requestInfos["path"].substr(pos);
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

void Request::collector(std::string &token){
    std::size_t separatorPos;

    separatorPos = token.find(": ");
    if(separatorPos != std::string::npos){
        _requestInfos.insert(std::pair<std::string , std::string>(toLowercase(token.substr(0, separatorPos)), toLowercase(token.substr(separatorPos + 2))));
    }
}

void Request::requestParser(std::string request,std::vector<Location> &locations){
    try{
        std::cout << "HERE ====================================" << std::endl;
        splitingHeaderBody(request);
        collectData();
        if (_headersParsed && !_requestLineParsed)
        {
            pathInCannonicalForm();
            matchingLocation(locations);
            isallowedMethod();
            checkingBadRequests();
        }
    } catch ( ClientError &e) {
        _status = e;
    }
}

std::map<std::string , std::string> Request::getRequestInfo() const{
    return _requestInfos;
}

void Request::splitingHeaderBody(std::string &request){
    size_t it;

    if(_headersParsed)
        _body = request;
    else {
        it = request.find("\r\n\r\n");
        if (it != std::string::npos){
            _headers = request.substr(0, it);
            _body += request.substr(it+4);
            _headersParsed = true;
        }
        else
            _headers = request;
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
        if (_requestInfos.find("transfer-encoding") == _requestInfos.end() && _requestInfos.find("content-length")==_requestInfos.end() && !_requestInfos["method"].compare("post"))
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

    for (std::vector<Location>::iterator   lIt = locations.begin(); lIt != locations.end(); lIt++) {
        std::string checks = _requestInfos["path"].substr(0, (*lIt).name.length());
        if ((*lIt).name == checks){
            found = true;
            this->_location = *lIt;
        }
    }
    if (!found)
        throw NOTFOUND;
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

void Request::readingBody(){
    std::map<std::string, std::string>::iterator it = _requestInfos.find("content-length");
    if (it != _requestInfos.end()) {
        size_t contentLength = std::stoi(it->second);
        if (contentLength > _body.length()) {
            throw LENGTHREQUIRED;
        }
    }
    else {
        it = _requestInfos.find("transfer-encoding");
        if (it != _requestInfos.end() && it->second == "chunked") {
            size_t pos = 0;
            while (pos < _body.length()) {
                size_t chunkSize = std::stoi(_body.substr(pos), 0, 16);
                if (chunkSize == 0) {
                    break;
                }
                pos = _body.find("\r\n", pos) + 2;
                if (pos == std::string::npos) {
                    throw BADREQUEST;
                }
                pos += chunkSize;
                if (pos > _body.length()) {
                    throw BADREQUEST;
                }
            }
        }
    }
}
