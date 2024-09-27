#include "../includes/parseConfigFile.hpp"

void Server::splitLocation(std::vector<std::string> &server_info)
{
	for (std::vector<std::string>::iterator it = server_info.begin(); it != server_info.end(); it++)
	{
		Location location;
		int braket = 0;
		std::vector<std::string> info = split(*it, ' ');
		if (info[0] == "location")
		{
			if (info.size() != 3 || (info[1][0] != '/' && info[1] != "*.php" && info[1] != "*.py") || info[2] != "{")
				throw std::invalid_argument("Syntax Error: invalid location");
			braket++;
			std::vector<std::string> rawlocation;
			rawlocation.push_back(info[1]);
			it = server_info.erase(it);
            while (it != server_info.end() && (*it)[0] != '}')
            {
                if (!((*it).empty()))
                    rawlocation.push_back(*it);
                it = server_info.erase(it);
            }
            location.r_location = rawlocation;
            _locations.push_back(location);
        }
        if ((*it)[0] == '}')
            *it = "";
    }
}

void checkSemiColon(std::vector<std::string> &info)
{
	int semi = 0;
	if (info[info.size() - 1][info[info.size() - 1].size() - 1] == ';')
	{
		for (std::vector<std::string>::iterator it = info.begin(); it != info.end(); it++)
		{
			std::string word = *it;
			for(int i = 0; i < word.length(); i++)
			{
				if (word[i] == ';')
					semi++;
			}
		}
	}
	else
		if (!info[0].empty())
			throw std::invalid_argument("Syntax Error: semicolon missing");
	if (semi > 1)
		throw std::invalid_argument("Syntax Error: more than one semicolon");
}

void Server::enterData(std::vector<std::string> &info)
{
	trim(info[info.size() - 1], ';');
	if (info[info.size() - 1].empty())
		info.erase(info.end() - 1);
	if (info.size() > 1 && info[0] == "listen")
		setPorts(info);
	else if (info.size() >= 2 && info[0] == "server_name")
		setServerName(info);
	else if (info.size() > 1 && info[0] == "host")
		setHost(info);
	else if (info.size() > 1 && info[0] == "error_page")
	{
		if (info.size() == 2 || info.size() == 3)
			setErrorPage(info);
		else
			throw std::invalid_argument("wrong number of arguments");
	}
	else if (info.size() > 1 && info[0] == "root")
	{
		if (info.size() == 2)
			this->rootPath = info[1];
		else
			throw std::invalid_argument("ERROR: root path is missing");
	}
	else if (info.size() >= 2 && info[0] == "index")
	{
		setIndex(info);
	}
	else if (info.size() > 1 && info[0] == "client_max_body_size")
		setClientMaxBodySize(info);
	else
		if (info.size() > 0 && !info[0].empty())
            throw std::invalid_argument("Syntax Error: unknow argument");
}

void Server::checkSyntaxe(std::vector<std::string> &server_info)
{
	server_info.erase(server_info.begin());
	for (std::vector<std::string>::iterator it = server_info.begin(); it != server_info.end(); it++)
	{
		std::vector<std::string> info = split(*it, ' ');
		checkSemiColon(info);
		enterData(info);
	}
}

void syntax_error(std::vector<Location> &locations)
{
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		Location &curr_location = *it;
		std::vector<std::string> loc = curr_location.r_location;
		(*it).name = loc[0];
		if (loc.size() < 2)
			throw std::invalid_argument("Syntax Error: Location block empty");
		for (std::vector<std::string>::iterator i = loc.begin() + 1; i != loc.end(); i++)
		{
			std::vector<std::string> info = split(*i, ' ');
			checkSemiColon(info);
			trim(info[info.size() - 1], ';');
			curr_location.setDataLocation(info);
		}
		
	}
}