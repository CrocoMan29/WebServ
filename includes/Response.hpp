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
# include "./parseConfigFile.hpp"
# define BUFFERSIZE 1024

class Response {
	private:
		std::map<int, std::string> code;
		std::map<std::string, std::string> mimetypes;
		// std::vector<std::string> autoindex;
		std::string type;
		std::string header;
		std::string path;
		std::string chunkSize;
		std::string method;
		int status;
		int socket;
	public:
		Response();
		Response(Request req, int socket);
		// Response(const Response& copy);
		// Response& operator=(const Response& rhs);
		~Response();
		std::string	getStatus(int code);
		void getContentType(std::string& path);
		bool validPath(std::string& path, std::string& root);
		bool isRegularFile(const std::string& path);
		bool isDirectory(const std::string& path);
		void postResponse(Request &request, Location &location);
		void setHeader();
		void chunk(Request& req);
		void setResponse(Request &req, int socket);
		void checkPath();
};

#endif
