#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include "Channel.hpp"
#include "Client.hpp"

class Client;
class Server {

	private:

	    int serverSocket;
	    sockaddr_in serverAddress;
	    int epollFd;
		int num;
	    std::string _passwd;
		std::string ServerName;
	    std::map<int, Client*> clients;
		std::map<std::string, Channel> channel;
	    void SetNonBlocking(int fd);
	    void HandleNewConnection();
	    void HandleClientMessage(int clientFd);
		void disconnectClient(int fd);
		void sendToClient(int fd, const std::string& response);
		void irssiParsingData(std::vector<std::string>::iterator begin, int ClientFD);

	public:

	    Server(int port, const std::string &passwd);
	    ~Server();
	    void InitServer();
	    void Bind();
	    void Listen();
	    void Run();
		void parseMessage(std::string message, int fd);
};

#endif
