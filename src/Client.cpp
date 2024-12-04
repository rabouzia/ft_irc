/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramzerk <ramzerk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:44:10 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/04 08:55:08 by ramzerk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <exception>


Client::Client(){}


void Client::InitClient()
{
	Socket = socket(AF_INET, SOCK_STREAM, 0);
	//server.InitServer();
}

void Client::Connect()
{
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in adresse_serveur; 
	adresse_serveur.sin_family = AF_INET; 
	adresse_serveur.sin_port = htons(8080); 
	adresse_serveur.sin_addr.s_addr = INADDR_ANY;

	if (connect(clientSocket, (struct sockaddr*)&adresse_serveur, sizeof(adresse_serveur)) == -1)
	{
		std::cout << "connect\n";
		throw std::exception();
	}
		
}

// void Client::ValidUser()
// {
// 	/*Chaque utilisateur n'a pas plus de neuf (9) caractères. Caractères disponibles dans le surnom:
// 	 a-z, A-Z, 0-9 et symboles spéciaux -, [, ], \, ^, {, }*/
// }

void Client::Send()
{
	std::string message;
	getline(std::cin, message); 
	std::cout << Socket << std::endl;
	send(Socket, static_cast<void *>(&message), message.size(), 0);
}

/*
int Client::Ping (std::string msg){

	
	send(_fd, (msg + "\r\n").c_str(), msg.length() + 2, MSG_NOSIGNAL);
	return 1;
}


*/