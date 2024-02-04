#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>

struct s_location
{
	
};Who yeah what


class NConfig{
	public:
		std::vector<std::string>server;
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