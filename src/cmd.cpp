/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdmessa <abdmessa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 00:08:11 by abdmessa          #+#    #+#             */
/*   Updated: 2024/12/07 00:08:12 by abdmessa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"
#include <vector>
class Server;

void Server::parsingData(std::string& str, int ClientFD) {
    std::vector<std::string> data = split(str, ' ');
    if (data.empty()) return;

    const std::string &command = data[0];
    if (command == "PASS") {
        handlePassCommand(data, ClientFD);
    } else if (command == "NICK") {
        handleNickCommand(data, ClientFD);
    } else if (command == "PING") {
        handlePingCommand(data, ClientFD);
    } else if (command == "PRIVMSG") {
        handlePrivmsgCommand(data, ClientFD);
    } else if (command == "JOIN") {
        handleJoinCommand(data, ClientFD);
    } else if (command == "MODE") {
        handleModeCommand(data, ClientFD);
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
    }
}

void Server::handlePassCommand(const std::vector<std::string>& data, int ClientFD) {
    if (data.size() < 2) {
        SendRPL(ClientFD, "461", "", "PASS :Not enough parameters");
        return;
    }
    const std::string &password = data[1];
    if (password == _passwd) {
        clientImap[ClientFD]->setPasswordVerified(true);
        clientSmap[clientImap[ClientFD]->getNick()] = clientImap[ClientFD];
        std::cout << "Correct Password\n";
    } else {
        SendRPL(ClientFD, "464", "", "Password incorrect.");
        disconnectClient(ClientFD);
        throw std::exception();
    }
}

void Server::handleNickCommand(const std::vector<std::string>& data, int ClientFD) {
    if (data.size() < 2) {
        SendRPL(ClientFD, "461", "", "NICK :Not enough parameters");
        return;
    }
    if (!clientImap[ClientFD]->GetPasswordVerified()) {
        SendRPL(ClientFD, "433", "", ":Password not verified");
        return;
    }
   	std::string newNick = data[1];
    clientSmap.erase(clientImap[ClientFD]->getNick());
    clientImap[ClientFD]->setNick(newNick);
    clientSmap[newNick] = clientImap[ClientFD];
    std::cout << "Nickname set to " << newNick << std::endl;

    std::string message = ":" + newNick + " NICK " + newNick + "\r\n";
    send(ClientFD, message.c_str(), message.size(), 0);
}

// Handle "PING" command
void Server::handlePingCommand(const std::vector<std::string>& data, int ClientFD) {
    if (data.size() < 2) {
        std::cerr << "Malformed PING message\n";
        return;
    }
    const std::string &token = data[1];
    std::string response = ":" + serverName + " PONG " + serverName + " :" + token + "\r\n";
    sendToClient(ClientFD, response);
}

// Handle "PRIVMSG" command
void Server::handlePrivmsgCommand( std::vector<std::string>& data, int ClientFD) {
    if (data.size() < 3) {
        SendRPL(ClientFD, "411", clientImap[ClientFD]->getNick(), ":No recipient or text provided");
        return;
    }
    std::string message = "";
    for (std::vector<std::string>::iterator it = data.begin(); it != data.end(); it++)
    {
        message += *it;
        message += " ";
    }
    const std::string &recipient = data[1];
    if (isAClient(recipient) == -1) {
        SendRPL(ClientFD, "401", clientImap[ClientFD]->getNick(), recipient + " :No such nick/channel");
        return;
    }
    std::string response = " PRIVMSG " + recipient + " " + message + "\r\n";
    std::cout << "Message sent :" << response << std::endl;
    send(clientSmap[recipient]->getSocket(), response.c_str(), response.size(), 0);
    send(clientImap[ClientFD]->getSocket(), response.c_str(), response.size(), 0);
}

// Handle "JOIN" command
void Server::handleJoinCommand(const std::vector<std::string>& data, int ClientFD) {
    if (data.size() < 2) {
        SendRPL(ClientFD, "461", clientImap[ClientFD]->getNick(), "JOIN :Not enough parameters");
        return;
    }
    const std::string &channelName = data[1];
    const std::string pass = (data.size() > 2) ? data[2] : "";

    if (!isChannel(channelName)) {
        channelSmap[channelName] = new Channel(channelName, pass, clientImap[ClientFD]);
        std::cout << "Created new channel: " << channelName << " with password: " << pass << std::endl;
    } else {
        std::cout << "Channel already exists: " << channelName << std::endl;
    }
}

void Server::handleModeCommand(const std::vector<std::string>& data, int ClientFD) {
    if (data.size() < 3) {
        SendRPL(ClientFD, "461", clientImap[ClientFD]->getNick(), "MODE :Not enough parameters");
        return;
    }
    const std::string &channelName = data[1];

    if (!isChannel(channelName)) {
        SendRPL(ClientFD, "403", clientImap[ClientFD]->getNick(), channelName + " :No such channel");
        return;
    }

    Channel *channel = channelSmap[channelName];
    const std::string &modeString = data[2];
    bool addingMode = (modeString[0] == '+');
    for (size_t i = 1; i < modeString.size(); ++i) {
        char mode = modeString[i];
        handleModeChange(channel, mode, addingMode, data, ClientFD);
    }
   // SendRPL(ClientFD, "324", clientImap[ClientFD]->getNick(), channelName + " " + channel->getModeString());
}

void Server::handleModeChange(Channel *channel, char mode, bool addingMode, const std::vector<std::string>& data, int ClientFD) {
    switch (mode) {
        case 'i': channel->setInviteOnly(addingMode, clientImap[ClientFD]); break;
        case 't': channel->setTopicRestricted(addingMode, clientImap[ClientFD]); break;
        case 'k': 
            if (addingMode && data.size() > 3) channel->setPassword(data[3], clientImap[ClientFD]);
            else channel->clearPassword(clientImap[ClientFD]); 
            break;
        case 'o': 
            if (data.size() > 3) (addingMode ? channel->addOperator(clientSmap[data[3]], clientImap[ClientFD]) : channel->removeOperator(clientSmap[data[3]], clientImap[ClientFD]));
            break;
        case 'l': 
            if (addingMode && data.size() > 3) channel->setUserLimit(std::atoi(data[3].c_str()), clientImap[ClientFD]);
            else channel->clearUserLimit(clientImap[ClientFD]); 
            break;
        default:
            SendRPL(ClientFD, "472", clientImap[ClientFD]->getNick(), std::string(1, mode) + " :is unknown mode char");
            break;
    }
}
