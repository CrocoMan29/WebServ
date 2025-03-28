# ifndef REQUEST_HPP
# define REQUEST_HPP

# include <fstream>
# include <iostream>
# include <map>
# include <vector>
#include <cstring>
# include <algorithm>
#include <cstring>
# include <string>
# include <time.h>
# include <sstream>
#include <sys/stat.h>   
#include <dirent.h>
#include <ctime>
#include <sys/wait.h>
#include <cstdio>
# include "./webServer.hpp"
# include "./parseConfigFile.hpp"
# define BUFFERSIZE 1024
# define CONSTRUCTREQUEST _status(200),_headersParsed(false),_bodyParsed(false),_isBadRequest(false), _clientMaxBodySize(0),_requestLineParsed(false), _bodySize(0), _chunckState(false), _checkingRequestInfo(false), _chunkSize(0), _chunkCRLF(false), _isPathSet(false), _timeOut(0)
# define CONSTRUCTPARAMREQ _rootPath(root), _index(index), _clientMaxBodySize(cmbs), _status(200),_headersParsed(false),_bodyParsed(false),_requestLineParsed(false),_isBadRequest(false), _bodySize(0), _chunckState(false), _checkingRequestInfo(false), _chunkSize(0), _chunkCRLF(false), _isPathSet(false), _timeOut(0)

// class Location;

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
    PAYLOADTOOLARGE = 413,
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
        size_t                              _chunkSize;
        bool                                _bodyParsed;
        bool                                _headersParsed;
        std::string                         _file;
        std::string                         _partialChunkSize;
        bool                                _chunckState;
        std::string                         _rootPath;
        bool                                _checkingRequestInfo;
        std::vector<std::string>            _index;
        bool                                _chunkCRLF;
        Location                            _location;
        int                                 _status;
        bool                                _requestLineParsed;
        bool                                _isPathSet;
        long                                _clientMaxBodySize;
        bool                                _isBadRequest;
		double								_timeOut;
        std::string                          abspath;
    public:
		clock_t								_time;
        Request();
        ~Request();
        Request(std::string root, std::vector<std::string> index, long clientMaxBodySize);
        Request(const Request &copy);
        Request &operator=(const Request &rhs);
        std::map<std::string , std::string> getRequestInfo() const;
        std::string urlDecode(const std::string &path);
        void RequestLineParser(const std::string& requestLine);
        void requestParser(const char* request,std::vector<Location> &locations, size_t readBytes);
        void collectData();
        void collector(std::string &request);
        void checkingBadRequests();
        void splitingHeaderBody(const char* request, size_t readBytes);
        void pathInCannonicalForm();
        void matchingLocation(std::vector<Location> &locations);
        void isallowedMethod();
        void bodyHandler();
        void extractingQuerryString();
        void readingBody(const char* request, size_t readBytes);
        void setChunkedBody(const char *body, size_t readBytes);
        void postChecker();
        void requestCleaner();
        void mentionAsBadReq(int e);
		void setTimeOut(double timeout){
			this->_timeOut = timeout;
		}
		double getTimeOut() const {
			return _timeOut;
		}
        bool isBadRequest() const{
            return _isBadRequest;
        };
        Location getLocation() const {
            return _location;
        };
        int getStatus() const {
            return _status;
        };
        std::string getExtension(const std::string &contentType){
            std::string extension = "";
            
            // std::cout << "content type: " << contentType << std::endl;
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
            else if (contentType == "video/mp4")
                extension = ".mp4";
            return extension;
        };
        std::string getHeader() const {
            return _headers;
        };
        std::string getAbspath() const {
            return abspath;
        };
        std::vector<std::string> getIndexes() const {
            return _index;
        }

        bool isRequestParsed() const {
            return _headersParsed && _bodyParsed && _requestLineParsed;
        };
        std::string getMethod() const {
            return this->_requestInfos.at("method");
        };
        std::string getPath() const {
            return this->_requestInfos.at("path");
        };
        std::string getVersion() const {
            return this->_requestInfos.at("version");
        };
        void setStatusCode(int status) {
            _status = status;
        };
        std::string getQueryString() const {
            if (this->_requestInfos.find("query") == this->_requestInfos.end())
                return "";
            return this->_requestInfos.at("query");
        };
        size_t      getBodySize() const {
            return _bodySize;
        };
        size_t      getContentLength() const {
            if (this->_requestInfos.find("content-length") == this->_requestInfos.end())
                return 0;
            return std::atol(this->_requestInfos.at("content-length").c_str());
        };
};


class Response {
	private:
		std::map<int, std::string> code;
		std::map<std::string, std::string> mimetypes;
		std::vector<std::string> indexFile;
		std::string type;
		std::string header;
		std::string path;
		std::string reqType;
		std::string chunkSize;
		std::string method;
		std::string querry;
		std::string cookies;
		std::ifstream file;
		std::string body;
		std::string generatedtPath;
		std::string scriptfile;
		std::string pathCgi;
        std::string cgiHeader;
        std::string postpath;
        std::string conLength;
		char **env;
		int status;
		int socket;
		bool readed;
		bool isError;
		bool isCGI;
		bool valueOfAutoIndex;
        bool isNot;
		Request req;
		int count;
		clock_t start;
		clock_t end;
        int cgistat;
		pid_t	pid;
        int    _isDeleted;
        double bodysize;
	public:
		bool finish;
		Response();
		Response(Request req, int socket);
		void sendResp(Request req, int socket);
		Response(const Response& copy);
		Response& operator=(const Response& rhs);
		~Response();
		std::string	getStatus(int code);
		std::string getContentType(std::string& path);
		bool isRegularFile(const std::string& path);
		bool isDirectory(const std::string& path);
		void postResponse(Request &request, Location &location);
		void setHeader();
		void chunk(Request& req);
		int checkPath(Request req);
		bool directoryHasFiles(const std::string& directoryPath);
		bool directoryHasIndexFile(const std::string& directoryPath);
		void listDir();
		void checkIndexFiles();
		std::string toString(long long nb);
		int	fillEnv();
		void	ft_free(char **env);
		int executeCgi(Request req);
		bool	getExt();
        long long fileSize(std::string path);
        void update(const Request &request);
        void del(const Request& request);
        void delete_directory(const std::string& path, const Request& request);
        void delete_file(const std::string& path);
        void deleteContent(const std::string& path);
        void    setStatus(int status){
            this->status = status;
        };
};



bool isFile(const char* path); 
bool isValidUri(std::string uri);
std::string toLowercase(std::string str);
std::string randomFileGenerator();

# endif