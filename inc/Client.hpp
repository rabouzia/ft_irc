/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramzerk <ramzerk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:15:52 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/04 09:03:28 by ramzerk          ###   ########.fr       */
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
#include <iostream>
// chaque username est unique de len max 9
#include <sys/epoll.h>
#include "Server.hpp"
#include <string>
#include <iostream>
class Server;


class Client{
	
	protected:
		std::string _username;
	
	public:
		int add_user(std::string name);
		bool is_inlist(std::string name);
		int Socket;
		sockaddr_in serverAddress;
		sockaddr_in clientAddress;
	public:
		Client();
		~Client(){};
		void Send();
		void Connect();
		void Accept();
		void InitClient();

};
#endif
