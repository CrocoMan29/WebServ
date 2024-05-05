#include "../includes/parseConfigFile.hpp"

void Conf::dataValidity(Server &server)
{
	std::vector<int> status_code;
	if (server.rootPath.empty())
		throw std::invalid_argument("invalid input: no root");
	if (server.host.empty())
		throw std::invalid_argument("invalid input: no host");
	if (server.ports.size() < 1)
		throw std::invalid_argument("invalid input: no port");
	if (server.index.empty())
		throw std::invalid_argument("invalid input: no index");
	for (std::vector<ErrorPage>::iterator it = server.error_page.begin(); it != server.error_page.end(); it++)
	{
		if (std::find(status_code.begin(), status_code.end(), (*it).status_code) != status_code.end())
			throw std::invalid_argument("invalid input: duplicate status code");
		status_code.push_back((*it).status_code);
	}
	std::vector<std::string> directive;
	for (std::vector<Location>::iterator it = server._locations.begin(); it != server._locations.end(); it++)
	{
		if (std::find(directive.begin(), directive.end(), (*it).name) != directive.end())
			throw std::invalid_argument("invalid input: duplicate directive");
		dataValidityLocation(*it, server.rootPath);
	}

	Location loc;
	loc.checkfile(server.error_page);
	// for (std::vector<int>::iterator it = status_code.begin(); it != status_code.end(); it++)
	// {
	// 	std::cout<< *it << std::endl;
	// }
}

void Conf::dataValidityLocation(Location &loc, std::string root)
{
	if (loc.root.empty())
		loc.root = root;
	if (loc.autoIndex == false)
		loc.autoIndex = false;
}

void Location::checkfile(std::vector<ErrorPage> &err)
{
	for (std::vector<ErrorPage>::iterator it = err.begin(); it != err.end(); it++)
	{
		std::ifstream file;
		file.open((*it).path.c_str());
		if (!file)
		{
			file.open((const char *)(root + (*it).path).c_str());
			if (!file)
				(*it).path = "./error/error.html";
		}
	}
}

void Conf::lastCheck(std::vector<Server> &servers)
{
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		for (std::vector<Server>::iterator iter = it + 1; iter != servers.end(); iter++)
		{
			betweenServers(*it, *iter);
		}
	}
}

void Conf::betweenServers(Server &server1, Server &server2)
{
	if (server1.host != server2.host)
		return ;
	for (std::vector<int>::iterator it = server1.ports.begin(); it != server1.ports.end(); it++)
	{
		if (std::find(server2.ports.begin(), server2.ports.end(), *it) != server2.ports.end())
		{
			server2.dup_port.push_back(*it);
			server2.is_duplicate = true;
			std::vector<int>::iterator iter = std::find(server2.ports.begin(), server2.ports.end(), *it);
			server2.ports.erase(iter);
			// std::cout << *iter << std::endl;
		}
	}
}