#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>

class errorPage{
	public:
		int status_code;
		std::string page;
		std::string path;
};

class location
{
	public:
		std::vector<std::string>r_location;
		std::string name;
		std::string root;
		bool upload_enable;
		std::string upload_store;
		std::vector<std::string>allowMethods;
		bool autoIndex;
		std::string index;
		std::string returnPath;
};
class server 
{
	public:
		std::vector<std::string> r_server;

		std::vector<int>listen;
		std::vector<std::string> server_name;
		std::string host;
		std::string rootPath;
		std::string index;
		std::string returnPath;
		long client_max_body_size;
		bool is_duplicate;
		std::vector<location> locations;
		std::vector<errorPage>error_page;
};

class WebServ{
	private:
		location location;
		server server;
		std::vector<std::string>comment;
		int c_bracket;
		std::vector<std::string>configFile;
	public:
		WebServ();
		void parseConfigFile(std::string &filename);
		void checkbracket(std::vector<std::string>&lines);
		void printConfFile(std::vector<std::string>&line);
		
};

std::vector<std::string> split(std::string s, char c);
std::string strtrim_semicolon(const std::string& str);
std::string trim(std::string &s, char c);