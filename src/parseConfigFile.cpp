#include "../includes/parseConfigFile.hpp"
// #include "parseConfigFile.hpp"

// #include "parseConfigFile.hpp"
std::string strtrim_semicolon(const std::string &str)
{
	size_t last = str.find_last_of(';');
    if (last != std::string::npos) {
        return str.substr(0, last);
    }
    return str;
}

std::string trim(std::string &s, char c)
{
	std::string  str;
	size_t index = s.find_first_not_of(c);
    if (index != std::string::npos)
        str += s.substr(index);
    index = str.find_last_not_of(c);
    if (index != std::string::npos)
	{
        str = str.substr(0, index + 1);
	}
	s = str;
	return (s);
}

std::vector<std::string> split(std::string s, char c)
{
    std::string  str;
    std::vector<std::string> splited;
    std::string  initial = s;
    int del=0,i=0,p=0,j=0;
    
	str = trim(initial, c);
    for (size_t i = 0; i < str.length(); i++)
		if (str[i] && str[i] == c && str[i + 1] != c)
			del++;
    while (del-- >= 0)
	{
		i = 0;
		while (str[p] && str[p] != c && str[p++])
			i++;
		splited.push_back(str.substr(j, i));
		while (str[p] && str[p] == c)
			p++;
		j = p;
	}
	return (splited);
}
Conf::Conf(){
	this->c_bracket = 0;
}

void Conf::parseConfigFile(std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file){
		throw std::runtime_error("ERROR: file not found");
	}
	std::string line;
	std::vector<std::string>lines;
	while (getline(file, line))
	{
		if (!line.empty())
			lines.push_back(line.append(" \n"));
	}
	checkbracket(lines);
	splitServers(lines, _servers);
	// for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	// {
	// 	printServer(&(*it));
	// }
	if (_servers.size() == 0)
		throw std::invalid_argument("ERROR: no server blocks");
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		parseServer(*it);

	}
	// std::vector<Server>::iterator it = _servers.begin();
	// parseServer(*it);
	// printConfFile(lines);
}
void Conf::checkbracket(std::vector<std::string>&lines)
{
	std::vector<std::string>::iterator it = lines.begin();
	while (it != lines.end())
	{
		trim(*it, ' ');
		if ((*it).size() > 0 && (*it)[0] == '#')
			it = lines.erase(it);
		else
			it++;
	}
	
	for (std::vector<std::string>::iterator iter = lines.begin(); iter != lines.end(); iter++)
	{
		std::vector<std::string> line = split(*iter, ' ');
		if (line[0] == "server")
		{
			// std::cout << "---i'm here---" << std::endl;
			if (line.size() > 2 && line[1] != "{")
				std::invalid_argument("ERROR: syntaxe error");
			c_bracket++;
		}
		else
		{
			for (std::vector<std::string>::iterator it = line.begin(); it < line.end(); it++)
			{
				if (*it == "{")
				{
					if (*(it - 2) == "location")
						c_bracket++;
				}
				else if (*it == "}")
					c_bracket--;
			}
			
		}
	}
	// std::cout << c_bracket << std::endl;
	if (c_bracket)
		throw std::invalid_argument("ERROR: bracket missing");
	
}

void Conf::splitServers(std::vector<std::string> &lines, std::vector<Server> &servers)
{
	std::vector<std::string> raw;
	for(std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		// std::cout << "--->" << *it << std::endl;
		std::vector<std::string> line = split(*it, ' ');
		if (*it == "server {")
			c_bracket++;
		else
		{
			for(std::vector<std::string>::iterator iter = line.begin(); iter != line.begin(); iter++)
			{
				if (*iter == "{")
					c_bracket++;
				else if (*iter == "}")
					c_bracket--;
			}
		}
		raw.push_back(*it);
		// std::cout << c_bracket << std::endl;	
		if (c_bracket == 0)
		{
			Server server;
			for (std::vector<std::string>::iterator itera = raw.begin(); itera != raw.end(); itera++)
			{
				// std::cout << "1-->" << *itera << std::endl;
				trim(*itera, ' ');
				trim(*itera, 9);
				trim(*itera, '\n');
				// std::cout << "2-->" << *itera << std::endl;
			}
			server.r_server = raw;
			server.is_duplicate = false;
			servers.push_back(server);
			raw.clear();
		}
	}
	// for (int i = 0; i < 10; i++)
	// {
	// 	std::cout << "i = " << i << std::endl;
	// 	printServer(servers[i]);
	// 	i--;
	// }
	// printServer(servers[1]);
}

void Conf::printConfFile(std::vector<std::string>&line){
	std::vector<std::string>::iterator it = line.begin();
	for (; it != line.end() ; it++)
	{
		std::cout << *it << std::endl;
	}
}

void Conf::parseServer(Server &server)
{
	server.splitLocation(server.r_server);
	
}
// void Conf::printServer(Server *server)
// {
// 	if (server)
// 	{
// 		int i = 0;
// 		std::cout << i << std::endl;
// 		for (std::vector<std::string>::iterator it = server->r_server.begin(); it != server->r_server.end(); it++)
// 		{
// 			std::cout<< "--->" << *it << std::endl;
// 		}
// 		i++;
// 		std::cout << i << std::endl;

// 	}
// }
