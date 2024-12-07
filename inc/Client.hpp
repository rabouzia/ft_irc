/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdmessa <abdmessa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:15:52 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/07 17:48:02 by abdmessa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/types.h>  // Types de données de base
#include <sys/socket.h> // Fonctions de gestion de socket
#include <netinet/in.h> // Structures pour les adresses Internet
#include <arpa/inet.h>  // Conversion d'adresses et autres utilitaires réseaux
#include <netdb.h>      // Fonctions de résolution de noms
#include <unistd.h>     // Fonctions POSIX comme close()
#include <string>
#include <iostream>

class Server;

class Client{
	
	protected:
		// Server server;
		int Socket;
		sockaddr_in serverAddress;
		sockaddr_in clientAddress;
		std::string _nick;
		bool psswdCheck;
		bool _InChannel;
		

	public:
		Client(int FD, std::string nick)
		{
			psswdCheck = false;
			_InChannel = false;
			Socket = FD;
			_nick =  nick;
		}
		~Client(){};
		void Send();
		void Connect();
		std::string getNick() const 
		{
			return _nick;
		}
		void setNick(const std::string& nick)
		{
			_nick = nick;
		}
		void InChannel(bool status) {_InChannel = status;}
		int getSocket() const {return Socket;}
		bool getInChannel() const {return _InChannel;}
		bool GetPasswordVerified();
		void InitClient();
		void setPasswordVerified(bool status);

};
#endif