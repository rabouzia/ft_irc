/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdembele <mdembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:15:52 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/05 19:01:38 by mdembele         ###   ########.fr       */
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
		bool psswdCheck;
		bool InChannel;

	public:
		Client(int FD, std::string nick)
		{
			psswdCheck = false;
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
		void setNick(std::string& nick)
		{_nick = nick;}
		int getSocket()
		{
			return Socket;
		}
		bool GetPasswordVerified();
		void InitClient();
		void setPasswordVerified(bool status);

};
#endif