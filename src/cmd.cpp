/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdmessa <abdmessa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 00:08:11 by abdmessa          #+#    #+#             */
/*   Updated: 2024/12/08 17:29:28 by abdmessa         ###   ########.fr       */
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
    } else if (command == "KICK") {
        handleKickCommand(data, ClientFD);
    } else if (command == "TOPIC") {
        handleTopicCommand(data, ClientFD);
    } else if (command == "INVITE") {
        handleInviteCommand(data, ClientFD);
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
    }
}

// KICK #test rabouzia 
void Server::handleKickCommand(const std::vector<std::string>& data, int ClientFD) {

     if (data.size() < 3) {
        SendRPL(ClientFD, "461", clientImap[ClientFD]->getNick(), "KICK :Not enough parameters");
        return;
    }

    const std::string& channelName = data[1];
    Channel* channel = NULL;
    
    // int targetFD = clientSmap[data[2]];

    std::map<std::string, Channel*>::iterator it = channelSmap.find(channelName);
    if (it == channelSmap.end()) {
        SendRPL(ClientFD, "403", clientImap[ClientFD]->getNick(), channelName + " :No such channel");
        return;
    }
    channel = it->second;
    // if (targetFD == -1 || !channel->isAClient(targetFD)) {
    //     SendRPL(ClientFD, "441", clientImap[ClientFD]->getNick(), kickName + " " + channelName + " :They aren't on that channel");
    //     return;
    // }
    if (!channel->isOperator(clientImap[ClientFD]->getSocket())) {
        SendRPL(ClientFD, "482", clientImap[ClientFD]->getNick(), channelName + " :You're not channel operator");
        channel->del(ClientFD);
        return;
    }
    else 
    {   
        std::string kickName = data[2];
        std::string kickMessage = ":" + clientImap[ClientFD]->getNick() + " KICK " + channelName + " " + kickName + "\r\n";
        std::map<int, Client*>::iterator clientIt;
        for (clientIt = channel->getClientMap().begin(); clientIt != channel->getClientMap().end(); ++clientIt) {
            send(clientIt->first, kickMessage.c_str(), kickMessage.size(), 0);
        }

        channel->del(clientSmap[kickName]->getSocket());
      //  send(ClientFD, kickMessage.c_str(), kickMessage.size(), 0);
    }
    // std::cout << "Kick " << channelName << " set to: " << KickName << std::endl;

}


// TOPIC #test :coucou
void Server::handleTopicCommand(const std::vector<std::string>& data, int ClientFD) {
    if (data.size() < 3) {
        SendRPL(ClientFD, "461", clientImap[ClientFD]->getNick(), "TOPIC :Not enough parameters");
        return;
    }

    const std::string& channelName = data[1];
    Channel* channel = NULL;

    std::map<std::string, Channel*>::iterator it = channelSmap.find(channelName);
    if (it == channelSmap.end()) {
        SendRPL(ClientFD, "403", clientImap[ClientFD]->getNick(), channelName + " :No such channel");
        return;
    }
    channel = it->second;
    if (!channel->isOperator(clientImap[ClientFD]->getSocket())) {
        SendRPL(ClientFD, "482", clientImap[ClientFD]->getNick(), channelName + " :You're not channel operator");
        return;
    }
    std::string newTopic = data[2].substr(1); 
    channel->setTopic(newTopic);
    SendRPL(ClientFD, "332", clientImap[ClientFD]->getNick(), channelName + " :" + newTopic);
    std::cout << "Topic for channel " << channelName << " set to: " << newTopic << std::endl;
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
void Server::handlePrivmsgCommand(std::vector<std::string>& data, int ClientFD) {
    if (data.size() < 3) {
        SendRPL(ClientFD, "411", clientImap[ClientFD]->getNick(), ":No recipient or text provided");
        return;
    }
    std::string message = "";
    for (std::vector<std::string>::iterator it = data.begin(); it != data.end(); it++) {
        message += *it;
        message += " ";
    }
    const std::string &recipient = data[1];
    if (data[1][0] == '#') 
    {  
        std::string channelName = data[1];
        
        std::map<std::string, Channel*>::iterator it = channelSmap.find(channelName);
        if (it == channelSmap.end()) 
        {
           // SendRPL(ClientFD, "403", clientImap[ClientFD]->getNick(), channelName + " :No such channel");
            return;
        }   
        Channel* channel = it->second;
        if (channel->isAClient(ClientFD) == 0)
        {
            SendRPL(ClientFD, "404", clientImap[ClientFD]->getNick(), channel->getName() + " :Cannot send to channel");
            return;
        }
        std::string response = ":" + clientImap[ClientFD]->getNick() + " PRIVMSG " + recipient + " :" + message + "\r\n";
        std::cout << "Message sent to channel: " << response << std::endl;  
    
        std::map<int, Client*>& clientMap = channel->getClientMap();
        if (clientMap.empty()) 
        {
            std::cout << "No clients in channel: " << channelName << std::endl;
            return;
        }
        for (std::map<int, Client*>::iterator it = clientMap.begin(); it != clientMap.end(); ++it) 
        {
            int clientSocket = it->first;
            if (clientSocket != ClientFD) 
            {
                if (channel->isAClient(clientSocket)) {
                    std::cout << "Message sent to --->" << clientImap[clientSocket]->getNick() << std::endl;
                    send(clientSocket, response.c_str(), response.size(), 0);
                }
            }
        }
    }
    else if (isAClient(recipient) == -1) 
    {
        SendRPL(ClientFD, "401", clientImap[ClientFD]->getNick(), recipient + " :No such nick/channel");
        return;
    }
    else
    {
        std::cout << "\n\n JE SUIS LA +============= \n\n";
        std::string response = clientImap[ClientFD]->getNick() + " PRIVMSG " + recipient + " " + message + "\r\n";
        std::cout << "Message sent to client: " << response << std::endl;
        send(clientSmap[recipient]->getSocket(), response.c_str(), response.size(), 0);
        send(clientImap[ClientFD]->getSocket(), response.c_str(), response.size(), 0);
    }
}

void Server::handleJoinCommand(const std::vector<std::string>& data, int ClientFD) {
    
    if (data.size() < 2) {
        SendRPL(ClientFD, "461", clientImap[ClientFD]->getNick(), "JOIN :Not enough parameters");
        return;
    }
    const std::string &channelName = data[1];
    const std::string pass = (data.size() > 2) ? data[2] : "";

    if (!isChannel(channelName)) {
        channelSmap[channelName] = new Channel(channelName, pass, clientImap[ClientFD], this);
        std::cout << "Created new channel: " << channelName << " with password: " << pass << std::endl;
    } 
    else {
        if (channelSmap.find(channelName) != channelSmap.end()) {

            Channel *existingChannel = channelSmap[channelName];
            if (existingChannel->addClient(clientImap[ClientFD], pass) == 1) 
            {
                std::cout << "User " << clientImap[ClientFD]->getNick() << " joined existing channel: " << channelName << std::endl;
                SendRPL(ClientFD, "332", clientImap[ClientFD]->getNick(), channelName + " :" + channelSmap[channelName]->getTopic());
                //SendRPL(ClientFD, "353", clientImap[ClientFD]->getNick(), "= " + channelName + " :" + channelSmap[channelName]->getClientList());
               // SendRPL(ClientFD, "366", clientImap[ClientFD]->getNick(), channelName + " :End of /NAMES list.");
            }
        } 
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
   
    if (!channel->isOperator(ClientFD))
    {
		SendRPL(ClientFD,"482", clientImap[ClientFD]->getNick(), channel->getName() + " You're not channel operator");
        channel->del(ClientFD);
        return;
    }
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

void Server::handleInviteCommand(const std::vector<std::string>& data, int ClientFD) {
    if (clientImap[ClientFD]->getInChannel() == false)
        return;        
    if (data.size() < 2) {
        SendRPL(ClientFD, "461", clientImap[ClientFD]->getNick(), "INVITE :Not enough parameters");
        return;
    }
    Channel *chan;
    for (std::map<std::string, Channel*>::iterator it = channelSmap.begin(); it != channelSmap.end(); it++)
    {
        if (it->second->isAClient(ClientFD))
           chan = it->second;
        if (it->second->isOperator(ClientFD))
            break ;
        if (it == channelSmap.end())
        {
		    SendRPL(ClientFD,"482", clientImap[ClientFD]->getNick(), chan->getName() + " You're not channel operator");
            chan->del(ClientFD);
            return;
        }
    }
    
    const std::string &InviteName = data[1];

    if (isAClient(InviteName) == -1) {
        SendRPL(ClientFD, "401", clientImap[ClientFD]->getNick(), InviteName + " :No such nick");
        return;
    }
    std::cout << data[0] <<"  :" << data[1] <<std::endl;
    chan->addWhiteList(clientSmap[InviteName]->getSocket());
}


