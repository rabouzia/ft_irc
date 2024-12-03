/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdembele <mdembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:44:42 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/03 16:03:36 by mdembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <iostream>
Server::Server(){}


void Server::InitServer()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
}

void Server::Bind()
{
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
		throw std::exception();
}


void Server::Listen()
{
	std::cout << "listened\n";
	if (listen(serverSocket, 5) == -1)
		throw std::exception();
}

void Server::Accept(std::string passwd)
{
	
}



int Server::setEpoll(std::string passwd)
{
	if(!check_pass(passwd))
		return 0;
	int epollFd = epoll_create1(0);
	serv_epoll.data.fd = serverSocket;
	if (serv_epoll.events = EPOLLIN)
	{
		listen();	
		epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, t_epoll);
		for(;;)
		{
			serv_epoll.data.fd = serverSocket;
			clientSocket = accept(serverSocket, NULL,NULL);
			if (clientSocket == - 1)
					throw std::exception();
			char buffer[1000] = {0};
			recv(clientSocket, buffer, sizeof(buffer), 0);
			std::cout << "Message from client: " << buffer
					<< std::endl;		
		}
	}
	else if (serv_epoll.events = EPOLLOUT)
	{
		//deco
	}
	else if () //personne nest co
	{
		

	}
	
}

sockaddr_in& Server::GetServerAddr()
{
	return (serverAddress);
}