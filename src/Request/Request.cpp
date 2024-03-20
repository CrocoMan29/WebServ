# include "../../includes/Request.hpp"
#include <cstring>

Request::Request(){

}

Request::~Request(){

}

void Request::readingRequest(std::ifstream &infile){
    if(infile.is_open()){
        char buffer[1024];
        infile.read(&buffer[0],1024);
        std::streamsize size = infile.gcount();
        std::string readed(buffer);
        splitingHeaderBody(readed);
        collectData();
    }
}

void Request::collectData(){
    int         lineNumber = 0;
    char        *token;

    try{
        token = strtok((char *)_headers.c_str(),"\n");
        while (token)
        {
            std::string stoken(token);
            lineNumber++;
            switch (lineNumber)
            {
                case 1 :
                    isValidHttpRequestLine(stoken);
                    break;
                default:
                    collector(stoken);
                    break;
            }
            token = strtok(NULL , "\n");
        }
    } catch (std::runtime_error &e) {
        this->_requestInfos.clear();
        std::cout<< e.what() << std::endl; 
    }
}
std::string toLowercase(std::string str) {
    std::string newStr(str);
    for (size_t i = 0; i < str.length(); ++i) {
        newStr[i] = std::tolower(newStr[i]);
    }
    return (newStr);
}

void Request::collector(std::string &token){
    std::size_t separatorPos;

    separatorPos = token.find(": ");
    if(separatorPos != std::string::npos){
        _requestInfos.insert(std::pair<std::string , std::string>(toLowercase(token.substr(0, separatorPos)), toLowercase(token.substr(separatorPos + 2))));
    }
}

std::map<std::string , std::string> Request::getRequestInfo() const{
    return _requestInfos;
}

void Request::splitingHeaderBody(std::string &request){
    size_t it;
    it = request.find("\n\n");
    if (it == std::string::npos){
        _body.clear();
        _headers = request;
    }
    else {
        _headers = request.substr(0, it);
        _body = request.substr(it+2);
    }
}


void Request::isValidHttpRequestLine(const std::string& requestLine) {
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
        throw std::runtime_error("Error");

    bool startsWithMethod = false;
    for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it) {
        if (parts[0] == *it) {
            startsWithMethod = true;
            _requestInfos.insert(std::make_pair("method", toLowercase(*it)));
            break;
        }
    }
    if (!startsWithMethod)
        throw std::runtime_error("Error");
    bool endsWithVersion = false;
    for (std::vector<std::string>::const_iterator it = versions.begin(); it != versions.end(); ++it) {
        if (parts[2] == *it) {
            endsWithVersion = true;
            _requestInfos.insert(std::make_pair("version", toLowercase(*it)));
            break;
        }
    }
    if (!endsWithVersion) {
        throw std::runtime_error("Error");
    }
    _requestInfos.insert(std::make_pair("path", parts[1]));
}

void Request::checkingBadRequests(){
    // Not implemented
    // std::map<std::string, std::string>::iterator it = _requestInfos.find("transfer-encoding");
    // if(it != _requestInfos.end() || it->second.compare("chunked"))
    //     throw NOTIMPLEMENTED;
    // if(_requestInfos["method"].compare("post") && 
    //     _requestInfos.find("transfer-encoding") == _requestInfos.end() &&
    //     _requestInfos.find("content-length") == _requestInfos.end())
    //     throw BADREQUEST;
    // if(_requestInfos["path"].length >= 2048)
    //     throw REQUESTURITOOLONG;
    
    // if(_requestInfos["path"])
}