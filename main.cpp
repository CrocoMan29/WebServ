#include "includes/parseConfigFile.hpp"
#include "includes/webServer.hpp"
# include "includes/Request.hpp" 

int main (int ac, char **av){
	if (ac != 2)
	{
		std::cerr << "too many argument" << std::endl;
		return (1);
	}
	std::string filename = av[1];
	// std::cout<< "filename = " << filename << std::endl; 
	try
	{
		Conf conf;
		conf.parseConfigFile(filename);
		webServ server(conf._servers);
		server.setUpServer();
		// std::cout << conf << std::endl;
		Request request;
		std::ifstream infile("tst.txt");

		request.readingRequest(infile);
		// std::cout << "Header \n"<<request.getHeader() << std::endl;
		// std::cout << "Body \n"<<request.getBody() << std::endl;
		// for(auto a : request.getRequestInfo()){
		// 	std::cout << a.first << ": " << a.second << std::endl;
		// }

		request.pathInCannonicalForm();
		request.matchingLocation(server);
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