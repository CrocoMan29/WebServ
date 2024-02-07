#include "../includes/parseConfigFile.hpp"

// #include "parseConfigFile.hpp"

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
WebServ::WebServ(){
	this->c_bracket = 0;
}

void WebServ::parseConfigFile(std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file){
		throw std::runtime_error("ERROR: file not found");
	}
	std::string line;
	std::vector<std::string>lines;
	if (!getline(file, line)){
		throw std::runtime_error("ERROR: file is empty");
	}
	lines.push_back(line.append(" \n"));
	while (getline(file, line))
	{
		if (!line.empty())
			lines.push_back(line.append(" \n"));
	}
	checkbracket(lines);
	printConfFile(lines);
}
void WebServ::checkbracket(std::vector<std::string>&lines)
{
	std::vector<std::string>::iterator it = lines.begin();
	while (it != lines.end())
	{
		trim(*it, ' ');
		if ((*it).size() > 1 && (*it)[0] == '#')
			it = lines.erase(it);
		else
			it++;
	}
	
	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
	{
		std::vector<std::string> line = split(*it, ' ');
		if (line[0] == "server")
		{
			// std::cout << "---i'm here---" << std::endl;
			if (line.size() > 2 && line[1] != "{")
				std::invalid_argument("ERROR: syntaxe error");
			c_bracket++;
		}
		else
		{
			for (std::vector<std::string>::iterator iter = line.begin(); iter < line.end(); iter++)
			{
				if (*iter == "{")
				{
					if (*(iter - 2) == "location")
						c_bracket++;
				}
				else if (*iter == "}")
					c_bracket--;
			}
			
		}
	}
	std::cout << c_bracket << std::endl;
	if (c_bracket)
		throw std::invalid_argument("ERROR: bracket missing");
	
}
std::string strtrim_semicolon(const std::string &str)
{
	size_t last = str.find_last_of(';');
    if (last != std::string::npos) {
        return str.substr(0, last);
    }
    return str;
}


void WebServ::printConfFile(std::vector<std::string>&line){
	std::vector<std::string>::iterator it = line.begin();
	for (; it != line.end() ; it++)
	{
		std::cout << *it << std::endl;
	}
	
}
