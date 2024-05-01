# ifndef REQUEST_HPP
# define REQUEST_HPP

# include <fstream>
# include <iostream>
# include <map>
# include <vector>
#include <cstring>
# include <algorithm>
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
        std::string                         _headers;
        std::string                         _body;
        std::vector<std::string>            _uriParts;
        size_t                              _read;
        int                                 _status;
        Location                            _location;
    public:
        Request();
        ~Request();
        std::map<std::string , std::string> getRequestInfo() const;
        void isValidHttpRequestLine(const std::string& requestLine);
        void requestParser(std::string request,std::vector<Location> &locations);
        void collectData();
        void collector(std::string &request);
        void checkingBadRequests();
        void splitingHeaderBody(std::string &request);
        void pathInCannonicalForm();
        void matchingLocation(std::vector<Location> &locations);
        void isallowedMethod();
        void readingBody();
        std::string getExtension(std::string path);
        std::string getHeader() const {
            return _headers;
        };
        std::string getBody() const ;
};

void isValidUri(std::string uri);
# endif