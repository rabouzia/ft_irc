/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdmessa <abdmessa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:13:48 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/07 00:26:10 by abdmessa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <map>
#include <list>
#include <string>

class Client;

class Channel {

	protected:

		std::map<int, Client*> ClientMap;
		std::map<int, Client*> OperatorMap;
		std::string _name;
		std::string _password;
		bool _inviteOnly;
		bool _topicAllow; //true
		bool _Islimit;
		int limitUser;
		

	public:	

		Channel(std::string name, std::string password, Client *);
		// void AddClient(Client *user, std::string password)
		// {
		// 	if (_password != password)
		// 	{
		// 		//RPL MESSAGE
		// 		return ;
		// 	}
			
		// }
		bool isOperator(int fd)
		{
			if (OperatorMap[fd])
				return true;
			return false;
		}
		void setInviteOnly(bool value, Client *OP) {
			//RPL MESSAGE
			std::cout << "\n je suis dans le i\n" << std::endl;
			(void)OP;
			_inviteOnly = value; 
		}
		void setTopicRestricted(bool value, Client *OP) { 
			//RPL MESSAGE
			(void)OP;
			std::cout << "\n je suis dans le t\n" << std::endl;
			
			_topicAllow = value; 
		}
		void setLimit(bool value, Client *OP) {
			//RPL MESSAGE
			(void)OP;
			std::cout << "\n je suis dans le L\n" << std::endl;

			_Islimit = value; 
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
};