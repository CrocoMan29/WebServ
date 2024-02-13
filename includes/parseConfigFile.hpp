#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string.h>

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

		void setDataLocation(std::vector<std::string> &info);
};
class Server 
{
	public:
		std::vector<std::string> r_server;

		std::vector<int>ports;
		std::vector<std::string> server_name;
		std::string host;
		std::string rootPath;
		std::string index;
		std::string returnPath;
		long client_max_body_size;
		bool is_duplicate;
		std::vector<Location> _locations;
		std::vector<ErrorPage>error_page;

		
		void splitLocation(std::vector<std::string> &server_info);
		void checkSyntaxe(std::vector<std::string> &server_info);
		void checkSemiColon(std::vector<std::string> &info);
		void checkHost(std::string &info);
		void enterData(std::vector<std::string> &info);
		void setPorts(std::vector<std::string> &info);
		void setServerName(std::vector<std::string> &info);
		void setHost(std::vector<std::string> &info);
		void setErrorPage(std::vector<std::string> &info);
		void setClientMaxBodySize(std::vector<std::string> &info);
		std::string getErrorPage(std::string path, std::string status);
		bool findCodes(int code);
		void syntax_error(std::vector<Location> &locations);
};

class Conf{
	public:
		std::vector<Server> _servers;
		// int c_bracket;
		// Conf();
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
std::string	to_String(int n);
std::map<int, std::string> initErrorPage();
int	ft_stoi(std::string str);
void printV(std::vector<std::string> &vector);
