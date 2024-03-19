# ifndef REQUEST_HPP
# define REQUEST_HPP

# include <fstream>
# include <iostream>
# include <map>
# include <vector>
#include <cstring>
# include <algorithm>
# include <sstream>

class Request
{
    private:
        std::map<std::string , std::string> _requestInfos;
        std::string                         _headers;
        std::string                         _body;
        size_t                              _read;
    public:
        Request();
        ~Request();
        std::map<std::string , std::string> getRequestInfo() const;
        void isValidHttpRequestLine(const std::string& requestLine);
        void readingRequest(std::ifstream &infile);
        void collectData();
        void requestLine(std::string &request);
        void collector(std::string &request);
        void splitingHeaderBody(std::string &request);
        std::string getHeader() const {
            return _headers;
        };
        std::string getBody() const {
            return _body;
        };
};

# endif