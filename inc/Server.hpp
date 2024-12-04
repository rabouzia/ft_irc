#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
// #include <sys/epoll.h>
#include <netinet/in.h>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>

class Server {

	private:

	    int serverSocket;
	    sockaddr_in serverAddress;
	    int epollFd;
	    std::string _passwd;
	    std::map<int, std::string> clients;
	    void SetNonBlocking(int fd);
	    void HandleNewConnection();
	    void HandleClientMessage(int clientFd);

	public:

	    Server(int port, const std::string &passwd);
	    ~Server();
	    void InitServer();
	    void Bind();
	    void Listen();
	    void Run();
};

#endif
