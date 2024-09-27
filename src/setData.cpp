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
}

void Server::setIndex(std::vector<std::string> &info)
{
	for(std::vector<std::string>::iterator it = info.begin() + 1; it != info.end(); it++)
	{
		if (std::find(this->index.begin(), this->index.end(), *it) == this->index.end())
			this->index.push_back(*it);
	}
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
}
bool Server::findCodes(int code)
{
	std::vector<int> codes;

    for (int i = 400; i < 420; i++)
        codes.push_back(i);
    for (int i = 421; i < 429; i++)
        codes.push_back(i);

    codes.push_back(431);
    codes.push_back(449);
    codes.push_back(450);
    codes.push_back(451);
    codes.push_back(456);
    codes.push_back(444);
    codes.push_back(495);
    codes.push_back(496);
    codes.push_back(497);
    codes.push_back(498);
    codes.push_back(499);


    for (int i = 500; i < 512; i++)
        codes.push_back(i);

    
    for (int i = 520; i < 528; i++)
        codes.push_back(i);
        
    if (std::find(codes.begin(), codes.end(), code) != codes.end())
        return true;
    return false;
}

std::string Server::getErrorPage(std::string path, std::string status)
{
	std::ifstream file;
    file.open(path.c_str());

    std::string line;
    std::string lines;

	while (getline(file, line))
		lines.append(line + '\n');
	if (path == "./error/error.html")
	{
		int pos = lines.find("404");
		lines[pos] = status[0];
		lines[pos + 1] = status[1];
		lines[pos + 2] = status[2];

		std::map<int, std::string> code_map = initErrorPage();
		if (code_map.find(atoi(status.c_str())) != code_map.end())
		{
			pos = lines.find("Not Found");
            int length = strlen("Not Found");
            std::string sub = lines.substr(0, pos);
            sub += code_map.find(atoi(status.c_str()))->second;
            pos += length;
            sub += lines.substr(pos, lines.length());
            lines = sub;
			
		}
	}
	return (lines);
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
			file.open(info[2].c_str());
			current.path = info[2];
			current.page = getErrorPage(current.path, to_String(status));
			if (!file)
			{
				current.path = "./error/error.html";
				current.page = getErrorPage(current.path, to_String(status));
			}
		}
		else
		{
			current.path = "./error/error.html";
			current.page = getErrorPage(current.path, to_String(status));
		}
		this->error_page.push_back(current);
	}
	else
		throw std::invalid_argument("Syntax error: invalid status code");
}

void Server::setClientMaxBodySize(std::vector<std::string> &info)
{
	if (info.size() != 3)
    {
        std::string sizeBody = info[1];

        if (sizeBody.size() == 1)
            throw std::invalid_argument("invalid max body size");

        char unit = tolower(sizeBody[sizeBody.size() - 1]);
        
        if (sizeBody.size() > 2 &&  (unit != 'm' && unit != 'k' && unit != 'b'))
            throw std::invalid_argument("invalid max body size");
        if (unit == 'k')
        {
            trim(sizeBody, 'k');
            client_max_body_size = ft_stoi(sizeBody);
            if (client_max_body_size <= 0)
                throw std::invalid_argument("invalid max body size");
            client_max_body_size = client_max_body_size * 1024;                
        }
        else if (unit == 'm')
        {
            trim(sizeBody, 'm');
            client_max_body_size = ft_stoi(sizeBody);
            if (client_max_body_size <= 0 )
                throw std::invalid_argument("invalid max body size");
            client_max_body_size = (client_max_body_size * 1024) * 1024;               
        }
        else if (unit == 'b')
        {
            trim(sizeBody, 'b');
            client_max_body_size = ft_stoi(sizeBody);
            if (client_max_body_size <= 0)
                throw std::invalid_argument("invalid max body size");
        }
        else
            throw std::invalid_argument("invalid max body size");
    }
    else
        throw std::invalid_argument("Syntax Error: wrong number of arguments");
}