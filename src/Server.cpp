/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdmessa <abdmessa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:10:55 by abdmessa          #+#    #+#             */
/*   Updated: 2024/12/06 21:53:41 by abdmessa         ###   ########.fr       */
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
    serverName = "The IRC";
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    int opt;
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
            {
                handleNewConnection();
            } 
            else if (eventsClient[i].events & EPOLLIN) 
            {
                handleClientMessage(eventsClient[i].data.fd);
            }
        }
    }
}


#include <sstream>

// Utility function to construct and send an IRC reply
void Server::SendRPL(int clientSocket, const std::string &replyCode, const std::string &nickname, const std::string &message) {
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



// void Server::ParsingData(std::string str, int ClientFD)
// {
// 	std::vector <std::string> data = split(str, ' ');
// 	std::vector <std::string>::iterator it = data.begin();
// 	std::cout << "SPLIT:" << *it << std::endl; 
// 	if (it->compare("PASS") == 0)
//     {
//         if ((it + 1)->compare(_passwd.c_str()) == 0) {
//             clientImap[ClientFD]->setPasswordVerified(true);
// 			clientSmap[clientImap[ClientFD]->getNick()] = clientImap[ClientFD];
//             std::cout << "Correct Password\n";
//         } 
//         else {
//             SendRPL(ClientFD, "464", clientImap[ClientFD]->getNick(), "Password incorrect.");
//             disconnectClient(ClientFD);
// 			throw std::exception();
//             return;
//         }
//     }
// 	else if ((it)->compare("NICK") == 0)
//     {
// 		std::cout << "Client fd ---->" << ClientFD << std::endl;
//         if (clientImap[ClientFD]->GetPasswordVerified()) 
// 		{
// 			if (!(it + 1)->empty())
// 			{
// 				std::string mess = ":" + *(it + 1) + " NICK " + *(it + 1) + "\r\n";
// 				send(ClientFD, mess.c_str(), mess.size(), 0);
// 				clientSmap.erase(clientImap[ClientFD]->getNick());
// 				clientImap[ClientFD]->setNick(*(it + 1));
// 				clientSmap[clientImap[ClientFD]->getNick()] = clientImap[ClientFD];
// 				std::cout <<  "This Nick has been seting " << *(it + 1) << std::endl;
// 			}
//         } 
//         else {
//             SendRPL(ClientFD, "433", clientImap[ClientFD]->getNick(), " :Nickname is already in use");
//             return;
//         }
//     }
// 	else if (it->compare("PING") == 0)
//     {
//         if (!(it + 1)->empty()) {
// 			std::cout << "PONG SEND\n";
//             std::string response = ":" + ServerName + " PONG " + ServerName + " :" + *(it + 1) + "\r\n";
//             sendToClient(ClientFD, response);
//         } else {
//             std::cerr << "Malformed PING message" << std::endl;
//         }
//     }
// 	else if (it->compare("PRIVMSG") == 0)
// 	{
// 		if(IsAclient(*(it + 1)) != -1)
// 		{
// 			std::cout << "FIND !!!\n";
// 			std::ostringstream response;
//     		response << " PRIVMSG " << clientImap[ClientFD]->getNick() << " " << *(it + 2) << "\r\n";
//     		std::ostringstream response2 ;
// 			response2 << " PRIVMSG " << clientImap[ClientFD]->getNick() << " " << *(it + 2) << "\r\n";
//    			std::string responseStr = response.str();
// 			send (clientSmap[*(it +1)]->getSocket(), responseStr.c_str() , responseStr.size(), 0);
// 			send (clientImap[ClientFD]->getSocket(), responseStr.c_str() , responseStr.size(), 0);
// 		}
// 		// else
// 			//send rpl error
// 	}
//     else if (it->compare("JOIN") == 0)
// 	{
// 		std::cout << "\n\n================JOINJOINJOIN===============\n\n" << std::endl;
//         if (IsChannel(*(it + 1)) == false)
//         {
//             std::cout << *(it + 1) << std::endl;
//             std::string pass;
//             if ((it + 2) != data.end())
//                 pass = *(it + 2);
//             else
//                 pass = "";        
//             channelSmap[*(it + 1)] = new Channel(*(it + 1), pass, clientImap[ClientFD]);
//             std::cout << "password is ===> " << pass << std::endl;
//         }
//         else
//             std::cout << "Channel deja existant\n" << std::endl;
// 	}
//     else if (it->compare("MODE") == 0) {
//     if ((it + 1) == data.end() || (it + 2) == data.end()) {
//         SendRPL(ClientFD, "461", clientImap[ClientFD]->getNick(), "MODE :Not enough parameters");
//         return;
//     }

//     std::string channelName = *(it + 1);
//     if (IsChannel(channelName) == false) {
//         SendRPL(ClientFD, "403", clientImap[ClientFD]->getNick(), channelName + " :No such channel");
//         return;
//     }

//     Channel *channel = channelSmap[channelName];
//     std::string modeString = *(it + 2);

//     bool addingMode = (modeString[0] == '+');
//     size_t i = 1; // Start from the second character
//     while (i < modeString.size()) {
//         char mode = modeString[i];
//         switch (mode) {
//             case 'i': // Invitation only
//                 channel->setInviteOnly(addingMode);
//                 break;

//             case 't': // Topic restricted
//                 channel->setTopicRestricted(addingMode);
//                 break;

//             case 'k': // Password
//                 if (addingMode) {
//                     if ((it + 3) == data.end()) {
//                         SendRPL(ClientFD, "461", clientImap[ClientFD]->getNick(), "MODE :Not enough parameters");
//                         return;
//                     }
//                     channel->setPassword(*(it + 3));
//                 } else {
//                     channel->clearPassword();
//                 }
//                 break;

//             case 'o': // Channel operator
//                 if (addingMode) {
//                     if ((it + 3) == data.end()) {
//                         SendRPL(ClientFD, "461", clientImap[ClientFD]->getNick(), "MODE :Not enough parameters");
//                         return;
//                     }
//                     channel->addOperator(*(it + 3));
//                 } else {
//                     if ((it + 3) == data.end()) {
//                         SendRPL(ClientFD, "461", clientImap[ClientFD]->getNick(), "MODE :Not enough parameters");
//                         return;
//                     }
//                     channel->removeOperator(*(it + 3));
//                 }
//                 break;

//             case 'l': // User limit
//                 if (addingMode) {
//                     if ((it + 3) == data.end()) {
//                         SendRPL(ClientFD, "461", clientImap[ClientFD]->getNick(), "MODE :Not enough parameters");
//                         return;
//                     }
//                     channel->setUserLimit(std::stoi(*(it + 3)));
//                 } else {
//                     channel->clearUserLimit();
//                 }
//                 break;

//             default:
//                 SendRPL(ClientFD, "472", clientImap[ClientFD]->getNick(), std::string(1, mode) + " :is unknown mode char");
//                 break;
//         }
//         i++;
//     }

//     SendRPL(ClientFD, "324", clientImap[ClientFD]->getNick(), channelName + " " + channel->getModeString());
// }
// 	else
//         return;
// }


// void Server::parseMessage(std::string message, int fd)
// {   


//     message = cleanIrssiString(message, '\r');
//     std::vector<std::string> splitMsg  = split(message, '\n');
//     std::vector <std::string>::iterator it = splitMsg.begin();
//     if (it->compare("PASS") == 0)
//     {
// 		it++;
//         if ((it)->compare(_passwd.c_str()) == 0) {
//             clientImap[fd]->setPasswordVerified(true);
//             std::cout << "Correct Password\n";
//         } 
//         else {
//             SendRPL(fd, "464", clientImap[fd]->getNick(), "Password incorrect.");
//             disconnectClient(fd);
//             return;
//         }
//     }
    
// }

void Server::handleClientMessage(int clientFd)
{
    char buffer[1024];
    int bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) {
        disconnectClient(clientFd);
        std::cout << "Client disconnected: " << clientFd << std::endl;
        return;
    }
    buffer[bytesRead] = '\0';
	std::string buff = cleanIrssiString(buffer, '\r');
	std::vector<std::string> tabData = split(buff, '\n');
	for (std::vector<std::string>::iterator it = tabData.begin(); it != tabData.end(); it++)
	{
		try {  parsingData(*it, clientFd);}
		catch (...){ return;}
		
	}
		
}
