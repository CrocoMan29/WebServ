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
			// std::cout << "yassir" << std::endl;
			std::cout << *it << std::endl;
            // while (it != server_info.end() && (*it)[0] != '}')
            // {
            //     if (!((*it).empty()))
            //         rawlocation.push_back(*it);
            //     it = server_info.erase(it);
            // }
            // location.r_location = rawlocation;
            // _locations.push_back(location);
        }
        // if ((*it)[0] == '}')
        //     *it = "";
    }
}