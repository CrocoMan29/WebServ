#include "includes/parseConfigFile.hpp"

int main (int ac, char **av){
	if (ac != 2)
	{
		std::cerr << "to many argument" << std::endl;
		return (1);
	}
	std::string filename = av[1];
	std::cout<< "filename = " << filename << std::endl; 
	try
	{
		WebServ webserv;
		webserv.parseConfigFile(filename);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	// root : ./
	// allow_method : GET POST DELETE;
	// index : time.py;
    // cgi_path : /usr/bin/python3 /bin/bash;
    // cgi_ext : .py .sh;
}