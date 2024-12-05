/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdmessa <abdmessa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:15:52 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/03 17:58:19 by abdmessa         ###   ########.fr       */
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
#include "Server.hpp"
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
		bool InChannel;
	public:
		Client();
		Client(int FD, std::string nick)
		{
			Socket = FD;
			_nick =  nick;
		}
		~Client(){};
		void Send();
		void Connect();
		std::string& getNick()
		{
			return _nick;
		}
		int getSocket()
		{
			return Socket;
		}
		void InitClient();

};
#endif