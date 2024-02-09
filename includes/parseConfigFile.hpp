#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>

class ErrorPage{
	public:
		int status_code;
		std::string page;
		std::string path;
};

class Location
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
class Server 
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
		std::vector<Location> locations;
		std::vector<ErrorPage>error_page;

		
		void splitLocation(std::vector<std::string> &server_info);
};

class Conf{
	public:
		std::vector<Server> _servers;
		int c_bracket;
		Conf();
		void parseConfigFile(std::string &filename);
		void checkbracket(std::vector<std::string>&lines);
		void splitServers(std::vector<std::string>&lines, std::vector<Server>&servers);
		void printConfFile(std::vector<std::string>&line);
		void parseServer(Server &server);
		void printServer(Server *server);
		
};

std::vector<std::string> split(std::string s, char c);
std::string strtrim_semicolon(const std::string& str);
std::string trim(std::string &s, char c);