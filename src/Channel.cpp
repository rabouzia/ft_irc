/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdmessa <abdmessa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:44:37 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/07 18:38:07 by abdmessa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <algorithm>

Channel::Channel(std::string name, std::string password, Client *client, Server *server) : _name(name), _password(password) {
	ClientMap[client->getSocket()] = client;
	OperatorMap[client->getSocket()] = client;
    std::vector<int> Whitelist;
	client->InChannel(true);
	this->_topicAllow = true;
	this->server = server;
	
}

void Channel::addClient(Client *user, const std::string& password) {

    if (_inviteOnly == false)
    {
        if ( !_password.empty() && _password != password) {
                server->SendRPL(user->getSocket(), "475", user->getNick(), _name + " :Cannot join channel (+k)");
            return;
        }   
    }
    else
    {
        if(!isInWhiteList(user->getSocket()))
        {
            //rpl
            return;
        }
    }
    if (ClientMap.count(user->getSocket())) {
        //":localhost 443 " + channel + " " + nickname + " :is already on channel\r\n") // used
        server->SendRPL(user->getSocket(), "443", user->getNick(), _name + " :is already on channel");
        //std::cout << "User " << user->getNick() << " is already in the channel: " << _name << std::endl;
        return;
    }

    ClientMap[user->getSocket()] = user;
    std::cout << "User " << user->getNick() << " joined the channel: " << _name << std::endl;

    std::string joinMessage = ":" + user->getNick() + " JOIN " + _name + "\r\n";
    for (std::map<int, Client*>::iterator it = ClientMap.begin(); it != ClientMap.end(); ++it) {
        if (it->first != user->getSocket()) {
            send(it->first, joinMessage.c_str(), joinMessage.size(), 0);
        }
    }
}


void Channel::Invite(int sock, Client *user)
{
	if (! isOperator(user->getSocket()))
	{
		server->SendRPL(user->getSocket(),"482", user->getNick(), _name + " You're not channel operator");
        del(user->getSocket());
		return ;
	}
    (void)sock;
}
		
void Channel::addWhiteList(int ClientFD)
{
    for (std::vector<int>::iterator it = whiteListe.begin(); it != whiteListe.end(); it++)
    {
        if (*it == ClientFD)
            return;
    }
    whiteListe.push_back(ClientFD);
}
bool Channel::isInWhiteList(int ClientFD)
{
    for (std::vector<int>::iterator it = whiteListe.begin(); it != whiteListe.end(); it++)
    {
        if (*it == ClientFD)
            return true;
    }
    return (false);
}
