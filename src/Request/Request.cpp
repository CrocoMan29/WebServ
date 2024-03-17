# include "Request.hpp"
#include <cstring>

Request::Request(){

}

Request::~Request(){

}

void Request::readingRequest(std::ifstream &infile){
    if(infile.is_open()){
        char buffer[1024];
        // while (!infile.eof()) {
            infile.read(&buffer[0],1024);
            std::streamsize size = infile.gcount();
        // }
        std::string readed(buffer);
        collectData(readed);
        // std::cout << readed << std::endl;
    }
}

void Request::collectData(std::string &request){
    int         lineNumber = 0;
    char        *token;

    token = strtok((char *)request.c_str(),"\n");
    while (token)
    {
        std::string stoken(token);
        // std::cout << "token --- " << stoken << std::endl;
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

}

void Request::collector(std::string &token){
    std::size_t separatorPos;

    separatorPos = token.find(": ");
    if(separatorPos != std::string::npos){
        _requestInfos.insert(std::pair<std::string , std::string>(token.substr(0, separatorPos), token.substr(separatorPos + 2)));
    }
}

std::map<std::string , std::string> Request::getRequestInfo() const{
    return _requestInfos;
}

bool Request::isValidHttpRequestLine(const std::string& requestLine) {
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
    if (numSpaces != 2 || parts.size() != 3) {
        return false;
    }

    bool startsWithMethod = false;
    for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it) {
        if (parts[0] == *it) {
            startsWithMethod = true;
            _requestInfos.insert(std::make_pair("method", *it));
            break;
        }
    }
    if (!startsWithMethod) {
        return false;
    }
    bool endsWithVersion = false;
    for (std::vector<std::string>::const_iterator it = versions.begin(); it != versions.end(); ++it) {
        if (parts[2] == *it) {
            endsWithVersion = true;
            _requestInfos.insert(std::make_pair("version", *it));
            break;
        }
    }

    if (!endsWithVersion) {
        return false;
    }
    _requestInfos.insert(std::make_pair("path", parts[1]));
    return true;
}
void Request::requestLine(std::string &request){

}