#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <fstream>
# include <iostream>
# include <map>
# include <vector>
#include <cstring>
# include <algorithm>
# include <sstream>
#include <sys/stat.h>   
# include "../includes/Request.hpp"

class Response {
	private:
		std::map<int, std::string> code;
		std::map<std::string, std::string> mimetypes;
		std::string type;
		std::string header;
		std::string path;
		int status;
		int socket;
	public:
		Response();
		Response(const Response& copy);
		Response& operator=(const Response& rhs);
		~Response();
		std::string	getStatus(int code);
		std::string getContentType(std::string& path);
		bool validPath(std::string& path, std::string& root);
		bool isRegularFile(const std::string& path);
		bool isDirectory(const std::string& path);
		void postResponse(Request &request, Location &location);
};


#endif


#endif