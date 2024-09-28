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
	if (ac == 1)
	{
		filename = "./default.config";
	}
	else if (ac == 2)
	{

		filename = av[1];
	}else {
		std::cout << "Wrong number of argument !!!"<< std::endl;
		exit(1);
	}
	try
	{
		Conf conf;
		conf.parseConfigFile(filename);
		webServ server(conf._servers);
		server.setUpServer();
		
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