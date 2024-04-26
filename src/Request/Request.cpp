# include "../../includes/Request.hpp"
#include <cstring>

Request::Request(){

}

Request::~Request(){

}


void Request::collectData(){
    int         lineNumber = 0;
    char        *token;

    try{
        token = strtok((char *)_headers.c_str(),"\r\n");
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
            token = strtok(NULL , "\r\n");
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

void Request::requestParser(std::string request){
    splitingHeaderBody(request);
    // std::cout << "Header \n"<<getHeader() << std::endl;
	// std::cout << "Body \n"<<getBody() << std::endl;
	// for(auto a : getRequestInfo()){
	// 	std::cout << a.first << ": " << a.second << std::endl;
	// }
    collectData();
    pathInCannonicalForm();
}

std::map<std::string , std::string> Request::getRequestInfo() const{
    return _requestInfos;
}

void Request::splitingHeaderBody(std::string &request){
    size_t it;
    it = request.find("\r\n\r\n");
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

void isValidUri(std::string uri){
    if(uri.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$'()*+,;=%") != std::string::npos)
        throw std::invalid_argument("Invalid URI");
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

void Request::pathInCannonicalForm(){
    std::string                 path;
    char                        *token;

    token = strtok((char *)this->_requestInfos["path"].c_str(),"/");
    while (token){

        std::string stoken(token);

        if(stoken== ".") {

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
    std::cout<< this->_requestInfos["path"] << std::endl;

}

std::string Request::matchingLocation(webServ &server){
    std::vector<Server>::iterator   sIt;
    std::vector<Server>             servers;

    servers = server.getServers();
    for (std::vector<Server>::iterator sIt = servers.begin() ; sIt != servers.end(); sIt++){
        if(std::find(sIt->server_name.begin(),sIt->server_name.end(),this->_uriParts[0]) != sIt->server_name.end()){
            if(this->_uriParts.size() == 1){
                this->_requestInfos["path"] = "/" + this->_uriParts[0];
                return "/" + this->_uriParts[0];
            }
            for (std::vector<Location>::iterator lIt = sIt->_locations.begin(); lIt != sIt->_locations.end(); lIt++) {
                
            }
        }
        
    }
    return "";
}