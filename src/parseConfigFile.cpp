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


void Conf::parseConfigFile(std::string &filename)
{
	std::ifstream file;
	file.open(filename.c_str());

	std::string line;
	std::vector<std::string>lines;
	if (!file.is_open()){
		throw std::runtime_error("ERROR: file not found");
	}
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
	int bracket = 0;
	std::vector<std::string>::iterator iter = lines.begin();
	while (iter != lines.end())
	{
		trim(*iter, ' ');
		if ((*iter).size() > 0 && (*iter)[0] == '#')
			iter = lines.erase(iter);
		else
			iter++;
	}
	
	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		std::vector<std::string> line = split(*it, ' ');
		if (line[0] == "server")
		{
			// std::cout << "---i'm here---" << std::endl;
			if (line.size() > 2 && line[1] != "{")
				throw std::invalid_argument("ERROR: syntaxe error");
			bracket++;
		}
		else
		{
			for (std::vector<std::string>::iterator iter = line.begin(); iter != line.end(); iter++)
			{
				if (*iter == "{")
				{
					if (*(iter - 2) == "location")
						bracket++;
				}
				else if (*iter == "}")
					bracket--;
			}
			
		}
	}
	// std::cout << c_bracket << std::endl;
	if (bracket != 0)
		throw std::invalid_argument("ERROR: bracket missing");
	
}

void Conf::splitServers(std::vector<std::string> &lines, std::vector<Server> &servers)
{
	int braket = 0;
    std::vector<std::string> raw;
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
    {
        std::vector<std::string> line = split(*it, ' ');
        if (it == lines.begin())
            braket++;
        else
        {
            for (std::vector<std::string>::iterator iter = line.begin(); iter != line.end(); iter++)
            {
                if (*iter == "{")
                    braket++;
                else if (*iter == "}")
                    braket--;
            }
        }
        raw.push_back(*it);
        if (braket == 0)
        {
            Server server;
            for (std::vector<std::string>::iterator itera = raw.begin(); itera != raw.end(); itera++)
            {
                trim(*itera, ' ');
                trim(*itera, 9);
                trim(*itera, '\n');
            }
            server.r_server = raw;
            server.is_duplicate = 0;
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
