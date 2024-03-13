#pragma once 

#include "parseConfigFile.hpp"
# include <stdio.h>
# include <sys/socket.h>
# include <unistd.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <poll.h>
# include <errno.h>
# include <arpa/inet.h>
# include <sys/time.h>
# include <cstring>

class Server;

class FdsInfo{
	public:
		std::vector<Server> my_server;
		std::vector<Server> all_servers;

		int serverSock;
		int port;
		int readLen;
		int headrLen;
		size_t totalSend;
		size_t totalRead;
		size_t byteLeft; 
};

class webServ{
	private:
		std::vector<Server> _servers;
		std::vector<Server> _serv;
		std::vector<Server> _fdsinfo;
	public:
		webServ(std::vector<Server> servers);
		void setUpServer();
		~webServ();
};