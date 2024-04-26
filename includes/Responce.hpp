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

class Response {
	private:
		std::map<int, std::string> code;
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
};


#endif