#include "../includes/parseConfigFile.hpp"

void Location::setDataLocation(std::vector<std::string> &info)
{
	if (info[info.size() - 1].empty())
		info.erase(info.end() - 1);
	if (info.size() > 1)
	{
		if (info[0] == "autoindex")
			setAutoIndex(info);
		else if (info[0] == "upload_enable")
			setUploadEnable(info);
		else if (info[0] == "upload_store")
		{
			setUploadStore(info[1]);
		}
		else if (info[0] == "allow_methods")
			setAllowMethodes(info);
		else if (info[0] == "index")
			setIndex(info);
		else if (info[0] == "root")
			setRoot(info);
		else if (info[0] == "return")
			setReturnPath(info);
		else if (info[0] == "fastcgi_pass")
			setFastCGIPass(info);
		else
			throw std::invalid_argument("Syntax Error: invalid argument");
	}
}

int Location::notTheCode(std::string code)
{
	if (code == "301" || code == "302" || code == "303" || code == "304" || code == "305" || code == "306" || code == "307" || code == "308")
		return 1;
	return 0;
}

void Location::setReturnPath(std::vector<std::string> &info)
{
	if (info.size() == 3)
	{
		if (!notTheCode(info[1]))
			throw std::invalid_argument("Syntax Error: wrong status code number");
		this->returnPath.append(info[1] + " ");
		this->returnPath.append(info[2]);
	}
	else
		throw std::invalid_argument("Syntax Error: to much arguments");
}

void Location::setAllowMethodes(std::vector<std::string> &info)
{
	std::vector<std::string>::iterator it = info.begin() + 1;
	for (; it != info.end(); it++)
	{
		if (*it == "POST")
		{
			if (std::find(this->allowMethods.begin(), this->allowMethods.end(), *it) != this->allowMethods.end())
				throw std::invalid_argument("Syntax Error: duplicate method");
			this->allowMethods.push_back(*it);
		}
		else if (*it == "GET")
		{
			if (std::find(this->allowMethods.begin(), this->allowMethods.end(), *it) != this->allowMethods.end())
				throw std::invalid_argument("Syntax Error: duplicate method");
			this->allowMethods.push_back(*it);
		}
		if (*it == "DELET")
		{
			if (std::find(this->allowMethods.begin(), this->allowMethods.end(), *it) != this->allowMethods.end())
				throw std::invalid_argument("Syntax Error: duplicate method");
			this->allowMethods.push_back(*it);
		}
	}
	
}

void Location::setAutoIndex(std::vector<std::string> &info)
{
	if (info.size() == 2)
	{
		if (info[1] == "on")
			this->autoIndex = true;
		else if (info[1] == "off")
			this->autoIndex = false;
		else
			throw std::invalid_argument("Syntax Error: unknow argument");
	}
	else
		throw std::invalid_argument("Syntax Error: to much arguments");
}

void Location::setUploadEnable(std::vector<std::string> &info)
{
	this->upload_enable = false;
	if (info.size() == 2)
	{
		if (info[1] == "on")
			this->upload_enable = true;
		else if (info[1] == "off")
			this->upload_enable = false;
		else
			throw std::invalid_argument("Syntax Error: unknow argument");
	}
	else
		throw std::invalid_argument("Syntax Error: to much arguments");
}

void Location::setUploadStore(std::string &info)
{
	this->upload_store = info;
}

void Location::setRoot(std::vector<std::string> &info)
{
	if (info.size() == 2)
		this->root = info[1];
	else
		throw std::invalid_argument("Syntax Error: to mush arguments");
}

void Location::setFastCGIPass(std::vector<std::string> &info)
{
	if (info.size() == 2)
		this->fastCgiPass = info[1];
	else
		throw std::invalid_argument("Syntax Error: to mush arguments");
}

void Location::setIndex(std::vector<std::string> &info)
{
	if (info.size() == 2)
	{
		if (!this->index.empty())
			throw std::invalid_argument("Syntax Error: duplicat argument");
		else
			this->index = info[1];
	}
	else
		throw std::invalid_argument("Syntax Error: to mush arguments");
}