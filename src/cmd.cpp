/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdembele <mdembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 00:08:11 by abdmessa          #+#    #+#             */
/*   Updated: 2024/12/10 22:40:15 by mdembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"
#include <vector>
#include <cstring>
#include "errmsg.hpp"
class Server;

void Server::parsingData(std::string& str, int ClientFD) {
    // Trim leading and trailing whitespace
    if (str.empty()) return;

    std::vector<std::string> data = split(str, ' ');
	for (std::vector<std::string>::iterator it  = data.begin(); it != data.end(); it++)
	{
		str = *it;
		for (size_t i = 0; i < str.length(); i++)
		{
			std::cout << "|" << str[i] << "|" << "   |" << static_cast<int>(str[i]) << "|" << std::endl; 
		}
		std::cout << "------------------------------------------------------" << std::endl;
	}
	std::cout << "------------------------------------------------------" << std::endl;
	std::cout  << std::endl;
	std::cout  << std::endl;
	std::cout << "------------------------------------------------------" << std::endl;
    if (data.empty()) return;

    const std::string &command = data[0];
    if (command == "PASS") {
        handlePassCommand(data, ClientFD);
    } else if (command == "NICK") {
        handleNickCommand(data, ClientFD);
    }
	else if (command == "USER") {
        handleUserCommand(data, ClientFD);
    } 
	else if (command == "PING") {
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
	}
    // } else if (command == "PART") {
    //     handlePartCommand(data, ClientFD);
    else if (command == "BOT") {
        handleBotCommand(ClientFD);
    }
	 else if (command == "QUIT") {
        handleQuitCommand(ClientFD);
    }  
	else {
        std::cerr << "Unknown command: " << command << std::endl;
    }
}

void Server::handleUserCommand( std::vector<std::string>& data,int  ClientFD)
{
	if (data.size() < 5)
	{
		SendRPL(ClientFD, "461", clientImap[ClientFD]->getNick(), "USER :Not enough parameters");
		return;
	}
	if (clientImap[ClientFD]->getNickVerified() == false)
	{
		SendRPL(ClientFD, "451", clientImap[ClientFD]->getNick(), "You have not registered");
		return;
	}
	clientImap[ClientFD]->_user = data[1];
	clientImap[ClientFD]->_mode = data[2];
	clientImap[ClientFD]->_realname = data[4];
	SendRPL(ClientFD, "001", clientImap[ClientFD]->getNick(), "Welcome to the IRC network, " + clientImap[ClientFD]->getNick() + "!");
	SendRPL(ClientFD, "002", clientImap[ClientFD]->getNick(), "Your host is " + serverName + ", running version 1.0");
	SendRPL(ClientFD, "003", clientImap[ClientFD]->getNick(), "This server was created on 2024/12/07 00:08:11");
	SendRPL(ClientFD, "004", clientImap[ClientFD]->getNick(), serverName + " 1.0 o o");
	clientImap[ClientFD]->setUserNameVerified(true);

}

void Server::handlePartCommand(const std::vector<std::string>& data, int ClientFD) {
    (void)data;
    Channel* currentChannel = NULL;
    for (std::map<std::string, Channel*>::iterator it = channelSmap.begin(); it != channelSmap.end(); it++) {
        if (it->second && it->second->isAClient(ClientFD)) {
            currentChannel = it->second;
            break;
        }
    }
    if (!currentChannel) {
		send(ClientFD, "442 :You're not on that channel\r\n", 30, 0);
        return;
    }

	std::string partMessage = ":" + clientImap[ClientFD]->getNick() + " PART " + currentChannel->getName() + "\r\n";
	if (currentChannel->isAClient(ClientFD) == 1)
	{
		currentChannel->del(ClientFD);
		std::cout << "Client " << clientImap[ClientFD]->getNick() << " left channel " << currentChannel->getName() << std::endl;
		for (std::map<int, Client*>::iterator it = (currentChannel->getClientMap()).begin(); it != (currentChannel->getClientMap()).end(); it++)
		{
			if (it->first != ClientFD)
				send(it->first, partMessage.c_str(), partMessage.size(), 0);
		}
	}
    if (currentChannel->getClientMap().empty()) {
        channelSmap.erase(currentChannel->getName());
        delete currentChannel;
        std::cout << "Channel " << currentChannel->getName() << " deleted as it is now empty." << std::endl;
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
    std::string kickName = data[2];
    if (!clientSmap[kickName] || !channel->isAClient(clientSmap[kickName]->getSocket()))
    {
        return;
    }
    else 
    {   
        std::string kickMessage = ":" + clientImap[ClientFD]->getNick() + " KICK " + channelName + " " + kickName + "\r\n";
        std::map<int, Client*>::iterator clientIt;
        for (clientIt = channel->getClientMap().begin(); clientIt != channel->getClientMap().end(); ++clientIt) {
            send(clientIt->first, kickMessage.c_str(), kickMessage.size(), 0);
        }
        channel->del(clientSmap[kickName]->getSocket());
		channel->decrementNbLimit();
    }
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
        channel->decrementNbLimit();
        return;
    }
    std::string newTopic = data[2].substr(1); 
    channel->setTopic(newTopic);
    std::map <int, Client *> clientMap = channel->getClientMap();
    for (std::map<int, Client*>::iterator it = clientMap.begin(); it != clientMap.end(); ++it) 
    {
        int clientSocket = it->first;
        if (channel->isAClient(clientSocket) == 1) {
            SendRPL(clientSocket, "332", clientImap[ClientFD]->getNick(), channelName + " :" + newTopic);
            std::cout << "Topic for channel " << channelName << " set to: " << newTopic << std::endl;
        }
    }
}

void Server::handlePassCommand(const std::vector<std::string>& data, int ClientFD) {
    if (data.size() < 2) {
        SendRPL(ClientFD, "461", "", "PASS :Not enough parameters");
        return;
    }
    const std::string &password = data[1];
    for (size_t i = 0; i < password.length(); i++)
    {
        std::cout << "|" << password[i] << "|" << std::endl;
    }
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
    if (clientImap[ClientFD] && !clientImap[ClientFD]->GetPasswordVerified()) {
        SendRPL(ClientFD, "433", "", ":Password not verified");
        return;
    }

    std::string newNick = data[1];

    // Validate nickname (example: no spaces, max 9 characters, etc.)
    if (newNick.empty() || newNick.length() > 9 || newNick.find(' ') != std::string::npos) {
        SendRPL(ClientFD, "432", "", ":Erroneous nickname");
        return;
    }

    // Check if the nickname is already in use
    if (clientSmap.find(newNick) != clientSmap.end()) {
        SendRPL(ClientFD, "433", "", ":Nickname is already in use");
        return;
    }
    // Update nickname
    std::string oldNick = clientImap[ClientFD]->getNick();
    clientSmap.erase(oldNick);
    clientImap[ClientFD]->setNick(newNick);
    clientSmap[newNick] = clientImap[ClientFD];
    std::string message = ":" + oldNick + "!user@host NICK :" + newNick + "\r\n";
	send(ClientFD, message.c_str(), message.size(), 0);
	for (std::map<int, Client*>::iterator it = clientImap.begin(); it != clientImap.end(); it++) {
		if (it->first != ClientFD) {
			send(it->first, message.c_str(), message.size(), 0);
		}
	}
   // send(ClientFD, message.c_str(), message.size(), 0); // Notify the client
	clientImap[ClientFD]->setNickVerified(true);
    std::cout << "Nickname changed from " << oldNick << " to " << newNick << std::endl;
}

// Handle "PING" command
void Server::handlePingCommand(const std::vector<std::string>& data, int ClientFD) {
    if (data.size() < 2) {
        std::cerr << "Malformed PING message\n";
        return;
    }
    const std::string &token = data[1];
    std::string response = ":" + serverName + " PONG " + ":" + token + "\r\n";
    sendToClient(ClientFD, response);
}

// Handle "PRIVMSG" command
void Server::handlePrivmsgCommand(std::vector<std::string>& data, int ClientFD) {
	if ((clientImap[ClientFD]->getNickVerified() == false )|| (clientImap[ClientFD]->getUserNameVerified() == false) || (clientImap[ClientFD]->GetPasswordVerified() == false))
	{
		std::cout << "NICK->>>>" << clientImap[ClientFD]->getNickVerified() << std::endl;
		std::cout << "USER->>>>" << clientImap[ClientFD]->getUserNameVerified() << std::endl;
		std::cout << "PASS->>>>" << clientImap[ClientFD]->GetPasswordVerified() << std::endl;	
		SendRPL(ClientFD, "451", clientImap[ClientFD]->getNick(), "You have not registered");
		return;
	}
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
            SendRPL(ClientFD, "403", clientImap[ClientFD]->getNick(), channelName + " :No such channel");
            return;
        } 
        Channel* channel = it->second;
        if (channel->isAClient(ClientFD) == 0)
        {
            // SendRPL(ClientFD,"482", clientImap[ClientFD]->getNick(), channel->getName() + " You're not channel operator");
            std::cout << "Not a client\n";
            return ;  
        }
		std::vector<std::string> data = split(message, ' ');
		message = data[2];
		for(size_t i = 3; i < data.size(); i++)
			message += " " + data[i];
        std::string response = RPL_PRIVMSG_CHANNEL(clientImap[ClientFD]->getNick(), channelName, message);
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
                if (channel->isAClient(clientSocket) == 1) {
                    std::cout << "Message sent to --->" << clientMap[clientSocket]->getNick() << std::endl;
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
		std::cout << "message " << message << std::endl;
		size_t colon_pos = message.find(':');
		message = message.substr(colon_pos);
        std::string response = clientImap[ClientFD]->getNick() + " PRIVMSG " + recipient + " " + message + "\r\n";
        std::cout << "Message sent to client: " << response << std::endl;
        send(clientSmap[recipient]->getSocket(), response.c_str(), response.size(), 0);
     //   send(clientImap[ClientFD]->getSocket(), response.c_str(), response.size(), 0);
    }
}

void Server::handleJoinCommand(const std::vector<std::string>& data, int ClientFD) {
    

	if ((clientImap[ClientFD]->getNickVerified() == false )|| (clientImap[ClientFD]->getUserNameVerified() == false) || clientImap[ClientFD]->GetPasswordVerified() == false)
	{
		SendRPL(ClientFD, "451", clientImap[ClientFD]->getNick(), "You have not registered");
		return;
	}
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
                SendRPL(ClientFD, "332", clientImap[ClientFD]->getNick(), channelName + " :" + channelSmap[channelName]->getTopic());
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
}

void Server::handleModeChange(Channel *channel, char mode, bool addingMode, const std::vector<std::string>& data, int ClientFD) {
   
    if (!channel->isOperator(ClientFD))
    {
		SendRPL(ClientFD,"482", clientImap[ClientFD]->getNick(), channel->getName() + " You're not channel operator");
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
            chan->decrementNbLimit();
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


void Server::handleBotCommand(int ClientFD)
{
	std::cout << "Bot command received\n";
	for(std::map<std::string, Client *>::iterator it = clientSmap.begin(); it != clientSmap.end(); it++)
	{
		if (clientImap[ClientFD] && it->second) {
			std::string response = ":" + serverName + " PRIVMSG " + it->second->getNick() + " :I AM A BOT WHO SEND MESSAGE TO EVERYONE EACH 5 MINUTES !!!\r\n";
			if(it->second &&  it->second->GetPasswordVerified() == true && it->second->getNickVerified() == true && it->second->getUserNameVerified() == true)
			{
				std::cout << "Bot message sent to " << it->second->getNick() << std::endl;
				if (send(it->second->getSocket(), response.c_str(), response.size(), 0) == -1) {
					std::cerr << "Error sending bot message to " << it->second->getNick() << std::endl;
				}
			}
		}
	}
}