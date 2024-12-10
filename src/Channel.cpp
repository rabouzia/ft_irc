/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdembele <mdembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:44:37 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/11 00:12:28 by mdembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <algorithm>

Channel::Channel(std::string name, std::string password, Client *client, Server *server) : _name(name), _password(password) {
    ClientMap[client->getSocket()] = client;
    OperatorMap[client->getSocket()] = client;
    whiteListe = std::vector<int>();
    client->InChannel(true);
    this->_topicAllow = true;
    this->server = server;
    this->_Islimit = false;
    this->nbLimit = 0;
    _inviteOnly = false;
    limitUser = 10;
}

int Channel::addClient(Client *user, const std::string& password) {

    if (_Islimit == true && nbLimit >= limitUser)
    {
        server->SendRPL(user->getSocket(), "471", user->getNick(), _name + " :Cannot join channel (+l)");
        return 0;
    }
    if (_inviteOnly == false)
    {
        if ( !_password.empty() && _password != password) {
                server->SendRPL(user->getSocket(), "475", user->getNick(), _name + " :Cannot join channel (+k)");
            return 0;
        }   
    }
    else
    {
        if(!isInWhiteList(user->getSocket()))
        {
            server->SendRPL(user->getSocket(), "473", user->getNick(), _name + " :Cannot join channel (+i)");
            return 0;
        }
    }
    if (ClientMap[user->getSocket()]) {

        server->SendRPL(user->getSocket(), "443", user->getNick(), _name + " :is already on channel");
        std::cout << "User " << user->getNick() << " is already in the channel: " << _name << std::endl;
        return 0;
    }
    
    ClientMap[user->getSocket()] = user;
    std::cout << "User " << user->getNick() << " joined the channel: " << _name << std::endl;
    this->nbLimit++;
    std::string joinMessage = ":" + user->getNick() + " JOIN " + _name + "\r\n";
    for (std::map<int, Client*>::iterator it = ClientMap.begin(); it != ClientMap.end(); ++it) {
            send(it->first, joinMessage.c_str(), joinMessage.size(), 0);
        }
//	send(user->getSocket(), "001 :Welcome to the channel\r\n", 27, 0);
//	send(user->getSocket(), "353 :Users in the channel \r\n", 27, 0);
    return 1;
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
