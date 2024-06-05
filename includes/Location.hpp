#pragma once

#include <iostream>
// #include <map>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <netinet/in.h>
#include "ErrorPage.hpp"

class Location
{
	public:
		std::vector<std::string>r_location;
		std::string name;
		std::string root;
		bool upload_enable;
		std::string upload_store;
		std::vector<std::string>allowMethods;
		bool autoIndex;
		std::string index;
		std::string returnPath;
		std::string fastCgiPass;

		void setDataLocation(std::vector<std::string> &info);
		void setUploadEnable(std::vector<std::string> &info);
		void setUploadStore(std::string &info);
		void setRoot(std::vector<std::string> &info);
		void setFastCGIPass(std::vector<std::string> &info);
		void setIndex(std::vector<std::string> &info);
		void setAllowMethodes(std::vector<std::string> &info);
		void setAutoIndex(std::vector<std::string> &info);
		void setReturnPath(std::vector<std::string> &info);
		int	 notTheCode(std::string code);
		void checkfile(std::vector<ErrorPage> &err);
};