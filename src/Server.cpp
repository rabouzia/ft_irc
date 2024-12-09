/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdembele <mdembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:10:55 by abdmessa          #+#    #+#             */
/*   Updated: 2024/12/09 16:37:38 by mdembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"
#include"utils.hpp"
#include <string.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include "errmsg.hpp"

Server::Server(int port, const std::string &passwd) : _passwd(passwd)
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
        throw std::runtime_error("Failed to create socket");
    serverName = "The IRC";
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    int opt;
    _old_buf = "";
    if(setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) { 
        throw std::runtime_error("Could not set socket options");    
    }
 
    if(setsockopt(this->serverSocket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) == -1) { 
        throw std::runtime_error("Could not set socket options");    
    }
    num = 0;
}

Server::~Server()
{
    close(serverSocket);
    close(epollFd);
}

void Server::setNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        throw std::runtime_error("Failed to get socket flags");
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        throw std::runtime_error("Failed to set socket to non-blocking");
}

void Server::initServer()
{
    setNonBlocking(serverSocket);
}

void Server::bindServ()
{
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
        throw std::runtime_error("Failed to bind socket");
}

void Server::listenServ()
{
    if (listen(serverSocket, 5) == -1)
        throw std::runtime_error("Failed to listen on socket");
    std::cout << "Server is listening on port " << ntohs(serverAddress.sin_port) << std::endl;
}

void Server::run()
{
    epollFd = epoll_create1(0);
    if (epollFd == -1)
        throw std::runtime_error("Failed to create epoll file descriptor");

    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = serverSocket;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) == -1)
        throw std::runtime_error("Failed to add server socket to epoll");

    epoll_event eventsClient[10];
    while (true) {
        int eventCount = epoll_wait(epollFd, eventsClient, 10, -1);
        if (eventCount == -1)
            throw std::runtime_error("epoll_wait failed");

        for (int i = 0; i < eventCount; ++i) {
            if (eventsClient[i].data.fd == serverSocket) 
                handleNewConnection();
            else if (eventsClient[i].events & EPOLLIN) 
                handleClientMessage(eventsClient[i].data.fd);
        }
    }
}

// Utility function to construct and send an IRC reply
void Server::SendRPL(int clientSocket, const std::string &replyCode, const std::string &nickname, const std::string &message) {
    std::ostringstream response;
    response << ":server " << replyCode << " " << nickname << " " << message << "\r\n";

    std::string responseStr = response.str();
    if (send(clientSocket, responseStr.c_str(), responseStr.size(), 0) == -1) {
        std::cerr << "Failed to send reply to client: " << clientSocket << std::endl;
    } else {
        std::cout << "Sent: " << responseStr;
    }
}

std::string intToString(int number) {
    std::stringstream ss;
    ss << number;
    return ss.str();
}

void Server::handleNewConnection() {
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket == -1)
        throw std::runtime_error("Failed to accept new connection");

    setNonBlocking(clientSocket);

    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = clientSocket;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) == -1)
        throw std::runtime_error("Failed to add client socket to epoll");
    std::cout << "New client connected: " << clientSocket << std::endl;

    // Send a welcome message to the new client
    std::string nickname = "NewUser"; // This should eventually be replaced with actual user nickname
     Client *newClient = new Client (clientSocket, (nickname + intToString(num++)));
    clientImap[clientSocket] = newClient;
    SendRPL(clientSocket, "001", clientImap[clientSocket]->getNick(), "Welcome to the IRC network, " + clientImap[clientSocket]->getNick() + "!");
}

std::string replaceIrssiString(std::string inputString, char oldChar, char newChar) {
    std::replace(inputString.begin(), inputString.end(), oldChar, newChar);
    return inputString;
}
std::string cleanIrssiString(std::string inputString, char c) {
    // Suppression des caractères \r et \n
    inputString.erase(
        std::remove(inputString.begin(), inputString.end(), c), 
        inputString.end()
 
);
     
    return inputString;
}

void Server::disconnectClient(int fd) {
    clientImap.erase(fd);
    epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, 0);
    std::cout << "Client disconnected: " << fd << std::endl;
    close(fd);
}


void Server::sendToClient(int fd, const std::string& message) {
    send(fd, message.c_str(), message.size(), 0);
}
#include <cstring>
// void Server::handleClientMessage(int clientFd)
// {
//     char buffer[1024];
    
//     int bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
//     if (bytesRead <= 0) {
//         disconnectClient(clientFd);
//         std::cout << "Client disconnected: " << clientFd << std::endl;
//         return;
//     }
//     buffer[bytesRead] = '\0';
//     std::string to = "";
//     to = buffer;
//     if (buffer[bytesRead - 1] != '\n' && buffer[bytesRead - 2] != '\r')
//     {
//         if (_old_buf.empty())
//             _old_buf = buffer;
//         else
//         {
//             _old_buf += buffer;
//         }
//     }
// 	std::string buff = cleanIrssiString(to.c_str(), '\r');
//     std::cout << buff << std::endl;
// 	parsingData(buff, clientFd);	
// }


void Server::handleClientMessage(int clientFd) {
    char buffer[1024];
    std::string completeMessage;

    // Lire jusqu'à obtenir un message complet ou une erreur
    int bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
        
    if (bytesRead <= 0) {
        disconnectClient(clientFd);
        return;
    }
    std::cout << "ByteRead ----> " <<  static_cast<int> (buffer[bytesRead - 1])  <<  static_cast<int> (buffer[bytesRead]) << std::endl;
    buffer[bytesRead] = '\0';
	if (buffer[bytesRead -1 ] != '\n' && buffer[bytesRead - 2] != '\r')
    {
        std::cout << buffer[bytesRead - 1] << std::endl;
		if (_old_buf.empty())
			_old_buf = buffer;
		else
        	_old_buf += buffer;
        return ;
    }
    completeMessage = _old_buf; 
    completeMessage += buffer;
    _old_buf = "";
    std::cout << "Received message: " << completeMessage << std::endl;
    completeMessage = cleanIrssiString(completeMessage, '\r');
	completeMessage[completeMessage.size()] = '\0';
	std::vector<std::string> data = split(completeMessage, '\n');
    for (std::vector<std::string>::iterator it = data.begin(); it != data.end(); it++)
	{
		*it = cleanIrssiString(*it, '\n');
    	parsingData(*it, clientFd);
	}
  
}
