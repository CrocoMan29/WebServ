#include "../includes/parseConfigFile.hpp"

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