#include "../includes/webServer.hpp"
#include "../includes/Request.hpp"
# include "../includes/Response.hpp"

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

void webServ::setUpServer(){
	Request request;
	// Response response;
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
		FdsInfo tmp;
		tmp.event.data.fd = _serv[i].socket_fd;
		tmp.event.events = EPOLLIN | EPOLLOUT;
		// fdData(tmp, _serv[i].socket_fd);
		if (guard(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _serv[i].socket_fd, &tmp.event), "epoll_ctl error") < 0)
			continue;
		_fdsinfo.push_back(tmp);
		fd_to_server[_serv[i].socket_fd] = &_serv[i];
	}
	while (true){
		int num_events;
		if (guard(num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1), "epoll_wait error") < 0)
			continue;
		for (int i = 0; i < num_events; i++)
		{
			Server* server = fd_to_server[events[i].data.fd];
			if (events[i].data.fd == server->socket_fd)
			{
				std::cout << "event fd : " << events[i].data.fd << ";" << std::endl;
				std::cout << "socket fd : " << server->socket_fd << " ]" << std::endl;
				client_socket = accept(server->socket_fd, (struct sockaddr *)&_serv[i]._address, (socklen_t *)&_serv[i].addrLen);
				if (client_socket > 0){
					if (fcntl(client_socket, F_SETFL, O_NONBLOCK) == -1)
						perror("fcntl");
				}
				std::cout << "new connection..............." << std::endl;
				FdsInfo tmp;
				tmp.event.events =  EPOLLIN | EPOLLOUT;
				tmp.event.data.fd = client_socket;
				if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &tmp.event) < 0)
					continue;
				_fdsinfo.push_back(tmp);
				fd_to_server[client_socket] = server;
			}
			if (events[i].events & EPOLLIN)
			{
				std::cout << "epoll in event fd : " << events[i].data.fd << ";" << std::endl;
				std::cout << "epoll in socket fd : " << _serv[i].socket_fd << " ]" << std::endl;
				// else
					// Server *server = fd_to_server[events[i].data.fd];
					// std::cout << "hellooooooooooooooooooo" << std::endl;
					client_socket = events[i].data.fd;
					char buffer[1024] = {0};
					int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
					if (bytes_received == -1) {
						// perror("recv");
					// exit(EXIT_FAILURE);
					} else if (bytes_received == 0) {
						std::cout << "Connection closed by client." << std::endl;
						// close(client_socket);
					} else {
						std::cout << "Received: " << buffer << std::endl;
						request.requestParser(buffer, _serv[i]._locations);
					}
				
				std::cout << "EPOLLIN: " << i << std::endl;
				std::cout << "request finished: " << request._requestLineParsed << std::endl;
			}
			else if (request._requestLineParsed == true && events[i].events & EPOLLOUT )
			{
				// Server *server = fd_to_server[events[i].data.fd];
				client_socket = events[i].data.fd;
				Response response(request, events[i].data.fd);
				response.setResponse(request, events[i].data.fd);
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