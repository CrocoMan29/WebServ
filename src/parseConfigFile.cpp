#include "../includes/parseConfigFile.hpp"

// #include "parseConfigFile.hpp"

WebServ::WebServ(){}

void WebServ::parseNginxLocation(std::istringstream &iss, NLocation &location)
{
	std::string key, value;
	while (iss >> key){
		std::string rest_of_line;
		std::getline(iss, rest_of_line);
		char* r_o_line = strdup(rest_of_line.c_str());
		char* token = std::strtok(r_o_line, " ");
		while (token != NULL){
			std::string splits;
			splits = token;
			if (std::strrchr(token, ';'))
				splits = strtrim_semicolon(splits);
			location.properties[key].push_back(splits);
			token = std::strtok(NULL, " ");
		}
	}
}
void WebServ::parseNgnixConfig(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open()){
		std::cerr << "ERROR: Could not open this file: " << filename << std::endl;
		return ;
	}
	std::string line;
	NLocation* currentlocation = NULL;
	if (!std::getline(file, line))
	{
		std::cerr << "ERROR: File is empty" << std::endl;
		return ;
	}
	if (line != "server {"){
		std::cerr << "ERROR" << std::endl;
		return ;
	}
	while (std::getline(file, line)){
		std::istringstream iss(line);
		std::string key;
		iss >> key;

		if (key.empty() || key[0] == '#')
			continue;
		if (key == "location") {
			std::string path;
			iss >> path;
			config.locationBlocks[path] = NLocation();
			currentlocation = &config.locationBlocks[path];
			parseNginxLocation(iss, *currentlocation);	
		} else if (currentlocation){
			parseNginxLocation(iss, *currentlocation);
		} else {
			std::string value;
			iss >> value;
			config.properties[key] = value;
		}
	}
}
void WebServ::printNgnixConfig()
{
	// for (std::map<std::string, std::string>::const_iterator it = config.properties.begin(); it != config.properties.end(); ++it) {
    //     std::cout << it->first << ": " << it->second << std::endl;
    // }

	for (std::map<std::string, NLocation>::const_iterator it = config.locationBlocks.begin(); it != config.locationBlocks.end(); ++it) {
        std::cout << "Location: " << it->first << std::endl;
        for (std::map<std::string, std::vector<std::string> >::const_iterator it2 = it->second.properties.begin(); it2 != it->second.properties.end(); ++it2) {
            std::cout << "  " << it2->first << " y: ";
            for (std::vector<std::string>::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
                std::cout << *it3 << " ";
            }
            std::cout << ";" << std::endl;
        }
    }
}
std::string strtrim_semicolon(const std::string &str)
{
	size_t last = str.find_last_of(';');
    if (last != std::string::npos) {
        return str.substr(0, last);
    }
    return str;
}
