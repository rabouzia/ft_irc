/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdmessa <abdmessa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:13:48 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/09 21:31:31 by abdmessa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include "Server.hpp"
# include <list>
# include <map>
# include <string>
# include <cstring>
# include <sstream>


class	Client;

class Channel
{
  protected:
	std::map<int, Client *> ClientMap;
	std::map<int, Client *> OperatorMap;
	Server *server;
	std::string _name;
	std::string _password;
	std::string _topic;
	bool _inviteOnly;
	bool _topicAllow;
	bool _Islimit;
	int limitUser;
	int nbLimit;
	std::vector<int> whiteListe;

  public:
	Channel(std::string name, std::string password, Client *, Server *);
	int addClient(Client *user, const std::string &password);
	std::string getPassword()
	{
		return (this->_password);
	}
	std::map<int, Client *> &getClientMap()
	{
		return (ClientMap);
	}
	void addWhiteList(int ClientFD);
	bool isInWhiteList(int ClientFD);

	void Invite(int sock, Client *user);
	void decrementNbLimit()
	{
		this->nbLimit--;
	}
	void del(int user)
	{
		if (ClientMap[user])
		{
			ClientMap[user]->InChannel(false);
			ClientMap.erase(user);
		}
	}
	bool isOperator(int fd)
	{
		if (OperatorMap[fd])
			return (true);
		return (false);
	}
	std::string getTopic()
	{
		return (this->_topic);
	};
	void setTopic(const std::string &newTopic)
	{
		_topic = newTopic;
	}

	void setInviteOnly(bool value, Client *OP)
	{
		std::string status = value ? "+i" : "-i";
		std::string message = ":" + OP->getNick() + " MODE " + getName() + " " + status + "\r\n";
		send(OP->getSocket(), message.c_str(), message.size(), 0);
		_inviteOnly = value;
	}

	void setTopicRestricted(bool value, Client *OP)
	{
		_topicAllow = value;
		if (OP)
		{
			std::string status = value ? "+t" : "-t";
			std::string message = ":" + OP->getNick() + " MODE " + getName() + " " + status + "\r\n";
			send(OP->getSocket(), message.c_str(), message.size(), 0);
		}
		if (OP)
		{
			std::string status = value ? "+l" : "-l";
			std::string message = ":" + OP->getNick() + " 324 " + getName() + " " + status + " :Limit mode " +
				(value ? "enabled" : "disabled") + "\r\n";
			send(OP->getSocket(), message.c_str(), message.size(), 0);
		}
	}



	void setPassword(std::string value, Client *OP)
	{
		_password = value;

		if (OP)
		{
        	std::cout << "enter in MODE [k]\n\n";
			std::string message = ":" + OP->getNick() + " 324 " + getName() + "+k :Password set\r\n";
			send(OP->getSocket(), message.c_str(), message.size(), 0);
		}
	}

	void clearPassword(Client *OP)
	{
		_password = "";
		if (OP)
		{
        	std::cout << "enter in MODE [-k]\n\n";
			std::string message = ":" + OP->getNick() + " 324 " + getName() + "-k :Password cleared\r\n";
			send(OP->getSocket(), message.c_str(), message.size(), 0);
		}
	}

	void addOperator(Client *user, Client *OP)
	{
        std::cout << "enter in MODE [+o]\n\n";
		if (!user || OperatorMap[user->getSocket()])
			return ;
		OperatorMap[user->getSocket()] = user;
		std::cout << "New operator => " << user->getNick() << std::endl;
		if (OP)
		{
			std::string message = ":" + OP->getNick() + " 324 " + getName() + "+o :Operator " + user->getNick() + " added\r\n";
			send(OP->getSocket(), message.c_str(), message.size(), 0);
		}
	}

	void removeOperator(Client *user, Client *OP)
	{
        std::cout << "enter in MODE [-o]\n\n";

		if (!user || OperatorMap.find(user->getSocket()) == OperatorMap.end())
			return ;
		OperatorMap.erase(user->getSocket());
		std::cout << "Operator removed => " << user->getNick() << std::endl;
		if (OP)
		{
			std::string message = ":" + OP->getNick() + " 324 " + getName() + "-o :Operator " + user->getNick() + " removed\r\n";
			send(OP->getSocket(), message.c_str(), message.size(), 0);
		}
	}
	void setUserLimit(int num, Client *OP)
	{
        std::cout << "enter in MODE [+l]\n\n";
		limitUser = num;
		_Islimit = true;
		if (OP)
		{
			std::stringstream ss;
			ss << num;
			std::string message = ":" + OP->getNick() + " 324 " + getName() + " +l " + ss.str() + " :User limit set\r\n";
			send(OP->getSocket(), message.c_str(), message.size(), 0);
		}
	}
	
	void clearUserLimit(Client* OP) 
	{
        std::cout << "enter in MODE [-l]\n\n";
		_Islimit = false;

		if (OP) 
		{
			std::string message = ":" + OP->getNick() + " 324 " + getName() + " -l :User limit cleared\r\n";
			send(OP->getSocket(), message.c_str(), message.size(), 0);
    	}
	}

	int isAClient(int fd)
	{
		if (ClientMap.find(fd) != ClientMap.end())
			return (1);
		return (0);
	}
	std::string getName()
	{
		return (_name);
	}

};

#endif