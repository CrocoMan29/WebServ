#include "../includes/parseConfigFile.hpp"

void Server::setPorts(std::vector<std::string> &info)
{
	for(std::vector<std::string>::iterator it = info.begin() + 1; it != info.end(); it++)
	{
		int port = ft_stoi(*it);
		if (port <= 0 || port > 65535)
			throw std::invalid_argument("ERROR: to large number for port");
		else
		{
			if (std::find(this->ports.begin(), this->ports.end(), port) != this->ports.end())
                throw std::invalid_argument("Syntax Error: duplicate port number");
            this->ports.push_back(port);
		}
	}
}

void Server::setServerName(std::vector<std::string> &info)
{
	for(std::vector<std::string>::iterator it = info.begin() + 1; it != info.end(); it++)
	{
		if (std::find(this->server_name.begin(), this->server_name.end(), *it) == this->server_name.end())
			this->server_name.push_back(*it);
	}
	// std::cout << server_name[0] << std::endl;
	// std::cout << server_name[1] << std::endl;
	// std::cout << server_name[2] << std::endl;
}

void Server::setHost(std::vector<std::string> &info)
{
	if (!this->host.empty())
		throw std::invalid_argument("Syntax Error: duplicate host");
	if (info.size() == 2)
		checkHost(info[1]);
	else
		throw std::invalid_argument("Syntax Error: more than one host");
}

void Server::checkHost(std::string &info)
{
	int point = 0;
	std::string check, host;
	if (info == "localhost")
	{
		this->host = "127.0.0.1";
		return ;
	}
	for (std::string::iterator it = info.begin(); it != info.end(); it++)
	{
		if (point <= 3 && isdigit(*it))
			check += *it;
		else if (*it == '.')
		{
			int number = ft_stoi(check);
			if (number > 255 || number < 0)
				throw std::invalid_argument("Syntax Error: invalid host");
			point++;
			host += check;
			host += '.';
			check.clear();
		}
		else
			throw std::invalid_argument("Syntax Error: invalid host");
	}
	if (point != 3)
		throw std::invalid_argument("Syntax Error: invalid host");
	int number = ft_stoi(check);
	if (number > 255 || number < 0)
		throw std::invalid_argument("Syntax Error: invalid host");
	host += check;
	this->host = host;
	// std::cout << this->host << std::endl;
}
//! find_codes not finished
bool Server::findCodes(int code)
{
	std::vector<int> codes;
	for (int i = 400; i < 420; i++)
		codes.push_back(i);
	if (std::find(codes.begin(), codes.end(), code) != codes.end())
		return true;
	return false;
}
//!setErrorPage not finished
void Server::setErrorPage(std::vector<std::string> &info)
{
	std::vector<int> saved_codes;

	int status = ft_stoi(info[1]);
	if (findCodes(status))
	{
		saved_codes.push_back(status);
		ErrorPage current;
		std::ifstream file;
		current.status_code = status;
		if (info.size() == 3)
		{
			file(info[2].c_str());
			current.path = info[2];
		}
	}
}