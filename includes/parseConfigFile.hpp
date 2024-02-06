#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>

struct s_location
{
	std::string name;
	std::vector<std::string>allowMethods;
	bool autoIndex = false;
	std::string index;
	std::string returnPath;
}t_location;
struct s_server
{
	int listen = 0;
	std::string server_name;
	std::string host;
	std::string rootPath;
	std::string index;
	std::pair<int, std::string>errorPage;
}t_server;


class NConfig{
	public:
		t_server s;
		t_location;
		std::vector<std::string>comments;
		
};

class WebServ{
	private:
		NConfig config;
	public:
		WebServ();
		void parseNginxLocation(std::istringstream &iss, NLocation &location);
		void parseNgnixConfig(const std::string &filename);
		void printNgnixConfig();
};

std::string strtrim_semicolon(const std::string& str);