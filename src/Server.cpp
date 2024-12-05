/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdembele <mdembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:10:55 by abdmessa          #+#    #+#             */
/*   Updated: 2024/12/05 17:09:16 by mdembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"
#include"utils.hpp"
#include <string.h>
#include <vector>
#include <algorithm>
#include "errmsg.hpp"
Server::Server(int port, const std::string &passwd) : _passwd(passwd)
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
        throw std::runtime_error("Failed to create socket");
    ServerName = "The IRC";
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    num = 0;
}

Server::~Server()
{
    close(serverSocket);
    close(epollFd);
}

void Server::SetNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        throw std::runtime_error("Failed to get socket flags");
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        throw std::runtime_error("Failed to set socket to non-blocking");
}

void Server::InitServer()
{
    SetNonBlocking(serverSocket);
}

void Server::Bind()
{
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
        throw std::runtime_error("Failed to bind socket");
}

void Server::Listen()
{
    if (listen(serverSocket, 5) == -1)
        throw std::runtime_error("Failed to listen on socket");
    std::cout << "Server is listening on port " << ntohs(serverAddress.sin_port) << std::endl;
}

void Server::Run()
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
            {
                std::cout << "new user---->" << eventsClient[i].data.fd <<std::endl;
                HandleNewConnection();
            } 
            else if (eventsClient[i].events & EPOLLIN) 
            {
                std::cout << "message client---->" << eventsClient[i].data.fd <<std::endl;
                HandleClientMessage(eventsClient[i].data.fd);
            }
        }
    }
}


#include <sstream>

// Utility function to construct and send an IRC reply
void SendRPL(int clientSocket, const std::string &replyCode, const std::string &nickname, const std::string &message) {
    std::ostringstream response;
    response << ":server " << replyCode << " " << nickname << " :" << message << "\r\n";

    std::string responseStr = response.str();
    if (send(clientSocket, responseStr.c_str(), responseStr.size(), 0) == -1) {
        std::cerr << "Failed to send reply to client: " << clientSocket << std::endl;
    } else {
        std::cout << "Sent: " << responseStr;
    }
}



#include <sstream>

std::string intToString(int number) {
    std::stringstream ss;
    ss << number;
    return ss.str();
}


void Server::HandleNewConnection() {
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket == -1)
        throw std::runtime_error("Failed to accept new connection");

    SetNonBlocking(clientSocket);

    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = clientSocket;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) == -1)
        throw std::runtime_error("Failed to add client socket to epoll");
    std::cout << "New client connected: " << clientSocket << std::endl;

    // Send a welcome message to the new client
    std::string nickname = "NewUser"; // This should eventually be replaced with actual user nickname
     Client *newClient = new Client (clientSocket, (nickname + intToString(num++)));
    clients[clientSocket] = newClient;
    SendRPL(clientSocket, "001", clients[clientSocket]->getNick(), "Welcome to the IRC network, " + clients[clientSocket]->getNick() + "!");
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
    clients.erase(fd);
    epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, 0);
    std::cout << "Client disconnected: " << fd << std::endl;
    close(fd);
}


void Server::sendToClient(int fd, const std::string& message) {
    send(fd, message.c_str(), message.size(), 0);
}

void Server::irssiParsingData(std::vector<std::string>::iterator begin, int ClientFD)
{
	std::vector <std::string>::iterator it = begin;
	if (it->compare("PASS") == 0)
    {
        if ((it + 1)->compare(_passwd.c_str()) == 0) {
			std::cout << "Correct Password1\n";
            clients[ClientFD]->setPasswordVerified(true);
            std::cout << "Correct Password2\n";
        } 
        else {
            SendRPL(ClientFD, "464", clients[ClientFD]->getNick(), "Password incorrect.");
            disconnectClient(ClientFD);
            return;
        }
    }
	else
	{
		SendRPL(ClientFD, "464", clients[ClientFD]->getNick(), "Password incorrect.");
        disconnectClient(ClientFD);
        return;
	}
	
}

void Server::parseMessage(std::string message, int fd)
{   


    message = cleanIrssiString(message, '\r');
	message = replaceIrssiString(message, '\n', ' ');
    std::vector<std::string> splitMsg  = split(message, ' ');
    std::vector <std::string>::iterator it = splitMsg.begin();

	//std::cout << "Parse message " << *it << " " << *(it + 1) << " " << *(it + 2) << std::endl;
	if (it->compare("CAP") == 0 && (it + 1)->compare("LS") == 0)
		irssiParsingData((it + 2), fd);
    else if (it->compare("PASS") == 0)
    {
        if ((it + 1)->compare(_passwd.c_str()) == 0) {
            clients[fd]->setPasswordVerified(true);
            std::cout << "Correct Password\n";
        } 
        else {
            SendRPL(fd, "464", clients[fd]->getNick(), "Password incorrect.");
            disconnectClient(fd);
            return;
        }
    }
    else if (it->compare("PING") == 0)
    {
        if (splitMsg.size() > 1) {
			std::cout << "PONG DANS TA MERE \n";
            std::string response = ":" + ServerName + " PONG " + ServerName + " :" + *(it + 1) + "\r\n";
            sendToClient(fd, response);
        } else {
            // Handle malformed PING message
            std::cerr << "Malformed PING message" << std::endl;
        }
    }
}

void Server::HandleClientMessage(int clientFd)
{
    char buffer[1024];
    int bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    //std::cout << " =========================== " << buffer << std::endl;
    // deco ou erreur
    if (bytesRead <= 0) {
        close(clientFd);
        epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, 0);
        clients.erase(clientFd);
        std::cout << "Client disconnected: " << clientFd << std::endl;
        return;
    }

    buffer[bytesRead] = '\0';
    std::string message(buffer);
    parseMessage(message, clientFd);

   // std::cout << "Message from client " << clientFd << ": " << message << std::endl;
  
    // Echo the message back to the client for testing
}
