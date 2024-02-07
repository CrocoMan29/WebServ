#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>

class errorPage{
	public:
		int id;
		std::string page;
		std::string path;
};

class location
{
	public:
		std::vector<std::string>name;
		std::vector<std::string>allowMethods;
		bool autoIndex;
		std::vector<std::string>index;
		std::vector<std::string>returnPath;
};
class server 
{
	public:
		std::vector<int>listen;
		std::vector<std::string> server_name;
		std::vector<std::string>host;
		std::vector<std::string>rootPath;
		std::vector<std::string>index;
		std::vector<errorPage>error;
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