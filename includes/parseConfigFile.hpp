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
		std::string fastCgiPass;

		void setDataLocation(std::vector<std::string> &info);
		void setUploadEnable(std::vector<std::string> &info);
		void setUploadStore(std::string &info);
		void setRoot(std::vector<std::string> &info);
		void setFastCGIPass(std::vector<std::string> &info);
		void setIndex(std::vector<std::string> &info);
		void setAllowMethodes(std::vector<std::string> &info);
		void setAutoIndex(std::vector<std::string> &info);
		void setReturnPath(std::vector<std::string> &info);
		int	 notTheCode(std::string code);
		void checkfile(std::vector<ErrorPage> &err);
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
		std::vector<int> dup_port;
		std::vector<Location> _locations;
		std::vector<ErrorPage>error_page;

		int _port;
        int socket_fd;
        struct sockaddr_in _address;

		
		void splitLocation(std::vector<std::string> &server_info);
		void checkSyntaxe(std::vector<std::string> &server_info);
		void checkHost(std::string &info);
		void enterData(std::vector<std::string> &info);
		void setPorts(std::vector<std::string> &info);
		void setServerName(std::vector<std::string> &info);
		void setHost(std::vector<std::string> &info);
		void setErrorPage(std::vector<std::string> &info);
		void setClientMaxBodySize(std::vector<std::string> &info);
		std::string getErrorPage(std::string path, std::string status);
		bool findCodes(int code);
		// void syntax_error(std::vector<Location> &locations);
};

class Conf{
	public:
		std::vector<Server> _servers;
		// int c_bracket;
		Conf();
		void parseConfigFile(std::string &filename);
		void checkbracket(std::vector<std::string>&lines);
		void splitServers(std::vector<std::string>&lines, std::vector<Server>&servers);
		void printConfFile(std::vector<std::string>&line);
		void parseServer(Server &server);
		void dataValidity(Server &server);
		void dataValidityLocation(Location &loc, std::string root);
		void lastCheck(std::vector<Server> &servers);
		void betweenServers(Server &server1, Server &server2);
		// void printServer(Server *server);
		
};
std::ostream &operator<<(std::ostream &os, const Conf &obj);
std::vector<std::string> split(std::string s, char c);
std::string strtrim_semicolon(const std::string& str);
std::string trim(std::string &s, char c);
std::string	to_String(int n);
std::map<int, std::string> initErrorPage();
int	ft_stoi(std::string str);
void printV(std::vector<std::string> &vector);
void syntax_error(std::vector<Location> &locations);
void checkSemiColon(std::vector<std::string> &info);
