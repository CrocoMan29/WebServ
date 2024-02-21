#include "../includes/parseConfigFile.hpp"
// #include "parseConfigFile.hpp"

// #include "parseConfigFile.hpp"

Conf::Conf(){
	
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
		dataValidity(*it);
	}
	lastCheck(_servers);
	// std::vector<Server>::iterator it = _servers.begin();
	// parseServer(*it);
	// it++;
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
            server.is_duplicate = false;
			// if (server.is_duplicate == true)
			// 	std::cout << "is duplicate = true" << std::endl;
			// else
			// 	std::cout << "is duplicate = false" << std::endl;
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
	server.client_max_body_size = -1;
	server.checkSyntaxe(server.r_server);
	syntax_error(server._locations);
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
