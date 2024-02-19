#include "../includes/parseConfigFile.hpp"
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

int	ft_stoi(std::string str)
{
	size_t	i;
	int	res;
	int	sign;

	i = 0;
	res = 0;
	sign = 1;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	if (i != str.length())
		return (-1);
	return (res * sign);
}

std::string	to_String(int n)
{
	std::stringstream tmp;

	tmp << n;

	return tmp.str();
}

std::map<int, std::string> initErrorPage()
{
	std::map<int, std::string> status_code;
    
	status_code[100] = "Continue";
	status_code[200] = "OK";
	status_code[201] = "Created";
	status_code[202] = "Accepted";
	status_code[204] = "No Content";
	status_code[300] = "Multiple Choices";
	status_code[301] = "Moved Permanently";
	status_code[302] = "Found";
	status_code[303] = "See Other";
	status_code[304] = "Not Modified";
	status_code[305] = "Use Proxy";
	status_code[307] = "Temporary Redirect";
	status_code[400] = "Bad Request";
	status_code[403] = "Forbidden";
	status_code[404] = "Not Found";
	status_code[405] = "Method Not Allowed";
	status_code[413] = "Payload Too Large";
	status_code[500] = "Internal Server Error";
	status_code[504] = "Gateway Timeout";
    
    return status_code;
}

void printV(std::vector<std::string> &vector)
{
	std::vector<std::string>::iterator it = vector.begin();
	for (; it != vector.end(); it++){
		std::cout << *it << std::endl;
	}
}

std::ostream &operator<<(std::ostream &os, const Conf &obj)
{
	for (int i = 0; i < obj._servers.size(); i++)
	{
		os << "=============Server==============" << std::endl;
		os << "------------" << i << "------------" << std::endl;
		os << "ports: ";
		for (std::vector<int>::const_iterator iter = obj._servers[i].ports.begin(); iter != obj._servers[i].ports.end(); iter++)
		{
			if (iter != obj._servers[i].ports.end() - 1)
				os << *iter << " ,";
			else
				os << *iter << "." << std::endl;
		}
		os << "server name: ";
		for (std::vector<std::string>::const_iterator iter = obj._servers[i].server_name.begin(); iter != obj._servers[i].server_name.end(); iter++)
		{
			if (iter != obj._servers[i].server_name.end() - 1)
				os << *iter << " ,";
			else
				os << *iter << "." << std::endl;
		}
		os << "host: " << obj._servers[i].host << std::endl;
		os << "root: " << obj._servers[i].rootPath << std::endl;
		os << "index: " << obj._servers[i].index << std::endl;
		os << "returnPath: " << obj._servers[i].returnPath << std::endl;
		os << "client_max_body_size: " << obj._servers[i].client_max_body_size << std::endl;
		if (obj._servers[i].is_duplicate == true)
			os << "is_duplicate: true" << std::endl;
		else
			os << "is_duplicate: false"<< std::endl;
		os << "dup_port: ";
		for (std::vector<int>::const_iterator iter = obj._servers[i].dup_port.begin(); iter != obj._servers[i].dup_port.end(); iter++)
		{
			if (iter != obj._servers[i].ports.end() - 1)
				os << *iter << " ,";
			else
				os << *iter << "." << std::endl;
		}
		for(int j = 0; j < obj._servers[i]._locations.size(); j++)
		{
			os << "_________________________________________________" << std::endl;
			os << "location " << obj._servers[i]._locations[j].name << "{" << std::endl;
			if (obj._servers[i]._locations[j].autoIndex == true)
				os << "     " << "autoindex: " << "true" << std::endl;
			else
				os << "     " << "autoindex: " << "false" << std::endl;
			os << "     root: " << obj._servers[i]._locations[j].root << std::endl;
			if (!obj._servers[i]._locations[j].index.empty())
				os << "     index: " << obj._servers[i]._locations[j].index << std::endl;
			if (!obj._servers[i]._locations[j].returnPath.empty())
				os << "     returnPath: " << obj._servers[i]._locations[j].returnPath << std::endl;
			if (!obj._servers[i]._locations[j].fastCgiPass.empty())
			os << "     fastCgiPass: " << obj._servers[i]._locations[j].fastCgiPass << std::endl;
			if (obj._servers[i]._locations[j].upload_enable == true)
			{
				os << "     upload_enable: true" << std::endl;
				os << "     upload_store: " << obj._servers[i]._locations[j].upload_store << std::endl;
			}
			else
			{
				os << "     upload_enable: false" << std::endl;
				os << "     upload_store: " << obj._servers[i]._locations[j].upload_store << std::endl;
			}
			os << "     allowMethdes: ";
			if (obj._servers[i]._locations[j].allowMethods.size() < 1)
			{
				for (std::vector<std::string>::const_iterator it = obj._servers[i]._locations[j].allowMethods.begin(); it != obj._servers[i]._locations[j].allowMethods.end(); it++)
				{
					if (it != obj._servers[i]._locations[j].allowMethods.end() - 1)
						os << *it << " ,";
					else
						os << *it << "." << std::endl;
				}
			}
			os << "}" << std::endl;
			os << "_________________________________________________" << std::endl;
		}
	}
	return os;
	
}
// void checkSemiColon(std::vector<std::string> &info)
// {
// }