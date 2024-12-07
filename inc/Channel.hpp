/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdmessa <abdmessa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:13:48 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/07 23:02:44 by abdmessa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP 
#define CHANNEL_HPP  

#include "Client.hpp"
#include <map>
#include <list>
#include <string>
#include "Server.hpp"

class Client;

class Channel {

	protected:
		std::map<int, Client*> ClientMap;
		std::map<int, Client*> OperatorMap;
		Server *server;
		std::string _name;
		std::string _password;
		std::string _topic;
		bool _inviteOnly;
		bool _topicAllow; //true
		bool _Islimit;
		int limitUser;
		std::vector<int> whiteListe;
		

	public:	

		Channel(std::string name, std::string password, Client *, Server *);
		void addClient(Client *user, const std::string& password);
		std::string getPassword() { return (this->_password); }
		std::map<int, Client*>& getClientMap() {return ClientMap;}
		void addWhiteList(int ClientFD);
		bool isInWhiteList(int ClientFD);

		void Invite(int sock, Client *user);
		void del(int user)
		{
			ClientMap.erase(user);	
		}
		bool isOperator(int fd)
		{
			if (OperatorMap[fd])
				return true;
			return false;
		}
		std::string getTopic() { return (this->_topic); };
		void setTopic(const std::string& newTopic) { _topic = newTopic;}

		void setInviteOnly(bool value, Client *OP) {
			//RPL MESSAGE
			std::cout << "\n je suis dans le i\n" << std::endl;
			(void)OP;
			_inviteOnly = value;
			std::cout << " value of invite boolean => " << this->_inviteOnly << std::endl;
		}

		void setTopicRestricted(bool value, Client *OP) { 
			//RPL MESSAGE
			(void)OP;
			std::cout << "\n je suis dans le t\n" << std::endl;
			
			_topicAllow = value; 
			std::cout << " value of topic boolean => " << this->_topicAllow << std::endl;

		}

		void setLimit(bool value, Client *OP) {
			//RPL MESSAGE
			(void)OP;
			std::cout << "\n je suis dans le L\n" << std::endl;

			_Islimit = value; 
			std::cout << " value of limit boolean => " << this->_Islimit << std::endl;

		}

		void setPassword(std::string value, Client *OP)
		{
			//RPL MESSAGE
			(void)OP;

			_password = value;
		}

		void clearPassword(Client *OP)
		{
			//RPL MESSAGE
			(void)OP;
			std::cout << "\n je suis dans le K\n" << std::endl;

			_password = "";
		}

		void addOperator(Client *user, Client *OP)
		{
			(void)OP;
			std::cout << "\n je suis dans le o\n" << std::endl;
			if(!user)
				return ;
			if (OperatorMap[user->getSocket()] )
				return ;
			if (!ClientMap[user->getSocket()] )
				return ;
			OperatorMap[user->getSocket()] = user;
			std::cout << " New operator => " << user->getNick() << std::endl;

		}

		void removeOperator(Client *user, Client *OP)
		{
			std::cout << "\n je suis dans le o\n" << std::endl;
			//RPL MESSAGE
			(void)OP;
			if(!user)
				return ;
			if (!OperatorMap[user->getSocket()] )
				return ;
			OperatorMap.erase(user->getSocket());
		}

		void setUserLimit(int num, Client *OP)
		{
			std::cout << "\n je suis dans le i\n" << std::endl;
			//RPL MESSAGE
			(void)OP;
			limitUser =  num;
			
		}
		void clearUserLimit(Client *OP)
		{
			std::cout << "\n je suis dans le i\n" << std::endl;

			//RPL MESSAGE
			(void)OP;
			_Islimit = false;
		}
		int isAClient(int fd)
		{
			if (ClientMap[fd])
				return 1;
			return 0;
		}
		std::string getName(){return _name;}
		
};

#endif