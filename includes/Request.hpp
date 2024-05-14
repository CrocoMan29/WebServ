# ifndef REQUEST_HPP
# define REQUEST_HPP

# include <fstream>
# include <iostream>
# include <map>
# include <vector>
#include <cstring>
# include <algorithm>
# include <string>
# include <time.h>
# include <sstream>
# include "./parseConfigFile.hpp"
# include "./webServer.hpp"
# include "./parseConfigFile.hpp"

enum ClientError{
    BADREQUEST = 400,
    UNAUTHORIZED = 401,
    PAYEMENTREQURED = 402,
    FORBIDDEN = 403,
    NOTFOUND = 404,
    METHODNOTALLOWED = 405,
    NOTACCEPTABLE = 406,
    PROXYAUTHENTIFICATIONREQUIRED = 407,
    REQUESTTIMEOUT = 408,
    CONFLICTS = 409,
    GONE = 410,
    LENGTHREQUIRED = 411,
    PRECONDITIONFAILED = 412,
    REQUESTENTITYTOOLARGE = 413,
    REQUESTURITOOLONG = 414,
    UNSUPORTEDMEDIATYPE = 415,
    REQUESTEDRANGNOTSATISFIABLE = 416,
    EXPECTATIONFAILED = 417,
};

enum ServerError {
    INTERNALSERVERERROR = 500,
    NOTIMPLEMENTED = 501,
    BADGATEWAY = 502,
    SERVICEUNAVAILABLE = 503,
    GATEWAYTIMEOUT = 504,
    HTTPVERSIONNOTSUPPORTED = 505
};

enum Successful {
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NONAUTHORITATIVEINFORMATION = 203,
    NOCONTENT = 204,
    RESETCONTENT = 205,
    PARTIALCONTENT = 206
};

enum Redirection {
    MULTIPLECHOICES = 300,
    MOVEDPERMANENTLY = 301,
    FOUND = 302,
    SEEOTHER = 303,
    NOTMODIFIED = 304,
    USEPROXY = 305,
    UNUSED = 306
};

class Request
{
    private:
        std::map<std::string , std::string> _requestInfos;
        std::vector<std::string>            _uriParts;
        std::string                         _headers;
        std::vector<char>                   _body;
        size_t                              _bodySize;
        bool                                _bodyParsed;
        bool                                _headersParsed;
        std::string                         _file;
        bool                                _chunckState;
        std::string                         _rootPath;
    public:
        Location                            _location;
        int                                 _status;
        bool                                _requestLineParsed;
        Request();
        ~Request();
        std::map<std::string , std::string> getRequestInfo() const;
        void RequestLineParser(const std::string& requestLine);
        void requestParser(const char* request,std::vector<Location> &locations, size_t readBytes, std::string root);
        void collectData();
        void collector(std::string &request);
        void checkingBadRequests();
        void splitingHeaderBody(const char* request, size_t readBytes, std::string root);
        void pathInCannonicalForm();
        void matchingLocation(std::vector<Location> &locations);
        void isallowedMethod();
        void bodyHandler();
        void readingBody(const char* request, size_t readBytes);
        void setChunkedBody(const char *body, size_t readBytes);
        void readChunk(char *line, size_t chunkSize);
        size_t isChunkSize(char *line);
        std::string getExtension(const std::string &contentType){
            std::string extension = "";
            
            std::cout << "content type: " << contentType << std::endl;
            if (contentType == "text/html")
                extension = ".html";
            else if (contentType == "text/css")
                extension = ".css";
            else if (contentType == "image/jpeg")
                extension = ".jpeg";
            else if (contentType == "image/png")
                extension = ".png";
            else if (contentType == "image/gif")
                extension = ".gif";
            else if (contentType == "image/svg+xml")
                extension = ".svg";
            else if (contentType == "application/javascript")
                extension = ".js";
            else if (contentType == "application/json")
                extension = ".json";
            else if (contentType == "application/xml")
                extension = ".xml";
            else if (contentType == "text/plain")
                extension = ".txt";
            else if (contentType == "application/pdf")
                extension = ".pdf";
            else if (contentType == "application/zip")
                extension = ".zip";
            else if (contentType == "video/webm")
                extension = ".webm";
            std::cout << "extension: " << extension << std::endl;

            return extension;
        };
        std::string getHeader() const {
            return _headers;
        };

        bool isRequestParsed() const {
            std::cout << "Request parsed: " << _headersParsed << _bodyParsed << _requestLineParsed << std::endl;
            return _headersParsed && _bodyParsed && _requestLineParsed;
        };
        std::string getBody() const;
        std::string getMethod() const {
            return this->_requestInfos.at("method");
        };
};

void isValidUri(std::string uri);
std::string toLowercase(std::string str);
std::string randomFileGenerator();

# endif