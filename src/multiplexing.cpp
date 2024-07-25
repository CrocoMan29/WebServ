#include "../includes/webServer.hpp"
#include "../includes/Request.hpp"
// # include "../includes/Response.hpp"


// std::map<int, Server*> fd_to_server;

webServ::webServ(std::vector<Server> servers){
	this->_servers = servers;
    for (size_t i = 0; i < servers.size(); i++)
    {
        for(size_t j = 0; j < servers[i].ports.size(); j++)
        {
            Server _servers = servers[i];
            _servers._port = servers[i].ports[j];
			_servers.addrLen = sizeof(_servers._address);
            _servers._address.sin_family = AF_INET;
            _servers._address.sin_addr.s_addr = inet_addr(servers[i].host.c_str());
            _servers._address.sin_port = htons(servers[i].ports[j]);
            memset(_servers._address.sin_zero, '\0', sizeof(_servers._address.sin_zero));
            this->_serv.push_back(_servers);
        }
    }
}

webServ::webServ(const webServ &obj){
	*this = obj;
}

webServ &webServ::operator=(const webServ &obj){
	if (this != &obj)
	{
		_serv = obj._serv;
		_servers = obj._servers;
		_fdsinfo = obj._fdsinfo;
		client_socket = obj.client_socket;
	}
	return *this;
}

void webServ::fdData(FdsInfo tmp, int fd)
{
	tmp.event.data.fd = fd;
	tmp.event.events = EPOLLIN | EPOLLOUT;
}

void webServ::acceptConnexion(int epoll_fd){
	struct epoll_event events[MAX_EVENTS];
	int new_socket;
	while (true){
		int num_events;
		if (guard(num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1), "epoll_wait error") < 0)
			exit(1);
		for (int i = 0; i < num_events; i++)
		{
			if (events[i].data.fd == _serv[i].socket_fd)
			{
				if ((new_socket = guard(accept(_serv[i].socket_fd, (struct sockaddr *)&_serv[i]._address, (socklen_t *)&_serv[i].addrLen), "accept error")) < 0)
					std::cout << "akhiiran " << std::endl;
					
			}
		}
	}
	std::cout << "connexion accepted" << std::endl;
	std::cout << _fdsinfo.size() << std::endl;
}

void webServ::destroySocket(int &epoll_fd, int &c_socket, Server &server){
	epoll_ctl(epoll_fd, EPOLL_CTL_DEL, c_socket, NULL);
	close(c_socket);
	server.requestMap.erase(c_socket);
	server.responseMap.erase(c_socket);
}

double webServ::takeTime(){
	clock_t time = clock();
	double stime =(double)time / CLOCKS_PER_SEC;
	return stime;
}

void webServ::setUpServer() {
	int noBind = 0;
	int epoll_fd = epoll_create1(0);
	struct epoll_event events[MAX_EVENTS];
	
	for (int i = 0; i < this->_serv.size(); i++)
	{
		int optval = 1;
		if ((this->_serv[i].socket_fd = guard(socket(AF_INET, SOCK_STREAM, 0), "socket error")) < 0)
			continue;
		if (guard(setsockopt(_serv[i].socket_fd, SOL_SOCKET, SO_REUSEADDR , &optval, sizeof(optval)), "setsockopt error") < 0)
			continue;
		if (guard(bind(_serv[i].socket_fd,
			(struct sockaddr *) &_serv[i]._address,
			_serv[i].addrLen), "bind error") < 0)
		{
			noBind++;
			if (noBind == (int)_serv.size())
				exit(1);
			continue;
		}
		if (guard(listen(this->_serv[i].socket_fd, 3), "listen error") < 0)
			continue;
		struct epoll_event event;
		event.data.fd = _serv[i].socket_fd;
		event.events = EPOLLIN | EPOLLOUT;
		// fdData(tmp, _serv[i].socket_fd);
		if (guard(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _serv[i].socket_fd, &event), "epoll_ctl error") < 0)
			continue;
		// _fdsinfo.push_back(tmp);
		fd_to_server[_serv[i].socket_fd] = _serv[i];
	}
	while (true){
		int num_events;
		if (guard(num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1), "epoll_wait error") < 0)
			continue;
		for (int i = 0; i < num_events; i++)
		{
			Server& server = fd_to_server[events[i].data.fd];
				std::cout << "event fd : " << events[i].data.fd << ";" << std::endl;
				std::cout << "socket fd : " << server.socket_fd << " ]" << std::endl;
			double currTime;
			if (events[i].data.fd == server.socket_fd)
			{

				client_socket = accept(server.socket_fd, (struct sockaddr *)&server._address, (socklen_t *)&server.addrLen);
				if (client_socket > 0){
					if (fcntl(client_socket, F_SETFL, O_NONBLOCK) == -1) {
						perror("fcntl");
					}
				}
				
				std::cout << "key outside :" << client_socket << std::endl;
				std::cout << "new connection..............." << std::endl;
				struct epoll_event event;
				event.events =  EPOLLIN | EPOLLOUT;
				event.data.fd = client_socket;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event) < 0)
					continue;
				server.requestMap.insert(std::pair<int, Request>(client_socket, Request(server.rootPath, server.index, server.client_max_body_size)));
				std::cout << "finish" << std::endl;
				// }
				server.requestMap[client_socket].setTimeOut(takeTime());
				// double time= takeTime();
				fd_to_server[client_socket] = server;
				std::cout << "time Out :  " << server.requestMap[client_socket].getTimeOut() << std::endl;
				continue;
			}
			currTime = takeTime() - server.requestMap[client_socket].getTimeOut();
			std::cout << "****current time****(outside) : " << currTime << std::endl;
			if (currTime >= 5)
			{
				std::cout << "****current time**** : (inside)" << currTime << std::endl;
				destroySocket(epoll_fd, client_socket, server);
				// epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_socket, NULL);
				// close(client_socket);
				// server.requestMap.erase(client_socket);
				// server.responseMap.erase(client_socket);
				std::cout << "time OUT done!!!!!!\n";
				// server.requestMap[client_socket].setTimeOut(takeTime());
			}
			else{
				int c_lient = events[i].data.fd;
				if ((events[i].events & EPOLLIN ))
				{
						// Server *server = fd_to_server[events[i].data.fd];
				        std::cout << "socket fd epollin: " << server.socket_fd << " ]" << std::endl;
						char buffer[1024] = {0};
						std::cout << "********** " << fd_to_server[c_lient].socket_fd << std::endl;
						std::cout << "########## " << c_lient << std::endl;
						int bytes_received = recv(c_lient, buffer, sizeof(buffer) -1, 0);
						if (bytes_received == -1) {
							destroySocket(epoll_fd, c_lient, server);
							// epoll_ctl(epoll_fd, EPOLL_CTL_DEL, c_lient, NULL);
							// close(c_lient);
							// server.requestMap.erase(c_lient);
							// server.responseMap.erase(c_lient);
							std::cout << "yassir is here : "  << c_lient << std::endl;
						}if (bytes_received == 0) {
							destroySocket(epoll_fd, c_lient, server);
							// epoll_ctl(epoll_fd, EPOLL_CTL_DEL, c_lient, NULL);
							// close(c_lient);
							// server.requestMap.erase(c_lient);
							// server.responseMap.erase(c_lient);
							std::cout << "Connection closed by client." << std::endl;
						}if (bytes_received > 0) {
							std::cout << "time Out :  " << server.requestMap[c_lient].getTimeOut() << std::endl;
							buffer[bytes_received] = '\0';
							server.requestMap[c_lient].requestParser(buffer, server._locations, bytes_received);
							server.responseMap.insert(std::pair<int, Response>(c_lient, Response()));
							std::cout << server.requestMap[c_lient].isRequestParsed() << std::endl;
						}
				}
				if (server.requestMap[c_lient].isRequestParsed() && (events[i].events & EPOLLOUT ))
				{
					std::cout << "----------hell------------" << std::endl;
					server.responseMap[c_lient].sendResp(server.requestMap[c_lient] ,c_lient);
					if (server.responseMap[c_lient].finish == true) {
						std::cout << "finished-------:" << std::endl;
						epoll_ctl(epoll_fd, EPOLL_CTL_DEL, c_lient, NULL);
						close(c_lient);
						server.requestMap.erase(c_lient);
						server.responseMap.erase(c_lient);
					}

				}
			}
		}
	}
}

int webServ::guard(int n, const char *er){
	if (n < 0)
	{
		perror(er);
		return (-1);
	}
	return(n);
}

webServ::~webServ() {}
