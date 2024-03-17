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
    public:
        Request();
        ~Request();
        std::map<std::string , std::string> getRequestInfo() const;
        bool isValidHttpRequestLine(const std::string& requestLine);
        void readingRequest(std::ifstream &infile);
        void collectData(std::string &request);
        void requestLine(std::string &request);
        void collector(std::string &request);
};

# endif