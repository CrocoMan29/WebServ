#include "../includes/parseConfigFile.hpp"

void Conf::dataValidity(Server &server)
{
	if (server.returnPath.empty())
		throw std::invalid_argument("invalid input: no root");
	if (server.host.empty())
		throw std::invalid_argument("invalid input: no host");
	if (server.ports.empty())
		throw std::invalid_argument("invalid input: no port");
	if (server.index.empty())
		throw std::invalid_argument("invalid input: no index");
	if (server.error_page.size() > 2)
		throw std::invalid_argument("invalid input: no error_page");
	if (server._locations.size() >= 1)
	{
		
	}
}