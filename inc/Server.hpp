/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdembele <mdembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:16:45 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/03 15:57:04 by mdembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP


#include <sys/types.h>  // Types de données de base
#include <sys/socket.h> // Fonctions de gestion de socket
#include <sys/epoll.h> // Fonctions de gestion de socket
#include <netinet/in.h> // Structures pour les adresses Internet
#include <arpa/inet.h>  // Conversion d'adresses et autres utilitaires réseaux
#include <netdb.h>      // Fonctions de résolution de noms
#include <unistd.h>     // Fonctions POSIX comme close()
#include <exception>
#include <string>

#define MAX_EVENT 10

class Server{
	
	protected:
		struct epoll_event t_epoll[MAX_EVENT];
		struct epoll_event serv_epoll;
		int serverSocket;
		int clientSocket;
		sockaddr_in serverAddress;
		sockaddr_in clientAddress;
		std::string passwd;
		std::string buffer;
	
	public:
		Server();
		~Server(){};
		int setEpoll(std::string passwd){};
		void Bind();
		void Listen();
		void Accept(std::string passwd);
		void InitServer();
		sockaddr_in& GetServerAddr();

};
#endif