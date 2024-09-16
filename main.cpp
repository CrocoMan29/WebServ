#include "includes/parseConfigFile.hpp"
#include "includes/webServer.hpp"
# include "includes/Request.hpp" 

void ignore_sigpipe() {
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGPIPE, &sa, NULL) == -1) {
        std::cerr << "Failed to ignore SIGPIPE: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
}

int main (int ac, char **av){
	// ignore_sigpipe();
	std::string filename;
	if (ac != 2)
	{
		filename = "./default.config";
	}
	else
		filename = av[1];
	// std::cout<< "filename = " << filename << std::endl; 
	try
	{
		Conf conf;
		conf.parseConfigFile(filename);
		webServ server(conf._servers);
		server.setUpServer();
		// std::cout << conf << std::endl;
		// request.readingRequest(infile);
		// // std::cout << "Header \n"<<request.getHeader() << std::endl;
		// // std::cout << "Body \n"<<request.getBody() << std::endl;
		// // for(auto a : request.getRequestInfo()){
		// // 	std::cout << a.first << ": " << a.second << std::endl;
		// // }

		// request.pathInCannonicalForm();
		// request.matchingLocation(server);
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