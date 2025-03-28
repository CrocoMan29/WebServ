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
# include <sys/epoll.h>
#include <fcntl.h>



class Server;
#define MAX_EVENTS 10
class FdsInfo{
	public:
		std::vector<Server> my_server;
		std::vector<Server> all_servers;

		int serverSock;
		struct epoll_event event;
};

class webServ{
	private:
		std::vector<Server> _servers;
		std::vector<Server> _serv;
		std::vector<FdsInfo> _fdsinfo;
		int client_socket;
	public:
		std::map<int, Server> fd_to_server;
		webServ(std::vector<Server> servers);
		webServ(const webServ &obj);
		webServ &operator=(const webServ &obj);
		double takeTime();
		void setUpServer();
		~webServ();
		std::vector<Server> getServers() const {
			return this->_servers ;
		}
		int guard(int n, const char *er);
		void destroySocket(int &epoll_fd, int &c_socket, Server &server);
};