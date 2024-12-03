/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdembele <mdembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:44:45 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/03 15:55:54 by mdembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"

class Server;
class Client;


int main (int ac, char **av)
{
	if (ac != 3)
		return(std::cout << "Usage ./ircserv <port> <passwd>" << std::endl, 1);
	
	Server a;
	int port = std::atoi(av[1]);
	std::string passwd = av[2];
	
	try{
		a.InitServer();
		a.Bind();
		a.Listen();
		if (!a.setEpoll(passwd))
			throw std::exception();
	}
	catch(...)
	{
		std::cout << "Server failed at initialization\n";
	}
	
}
	


// #include <string.h>
//  #include "Client.hpp"
//  int main ()
//  {
	
//  	 // creating socket
//     int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

//     // specifying address
//     sockaddr_in serverAddress;
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_port = htons(8080);
//     serverAddress.sin_addr.s_addr = INADDR_ANY;

//     // sending connection request
//     connect(clientSocket, (struct sockaddr*)&serverAddress,
//             sizeof(serverAddress));

//     // sending data
// 	std::string message;
//     getline(std::cin , message);
//     send(clientSocket, static_cast<void *>(&message), message.size(), 0);

//     // closing socket
//     close(clientSocket);

//     return 0;
// }
	

	
// }

/*

./ircserv <port> <password>

 port: The port number on which your IRC server will be listening to for incoming
IRC connections
• password: The connection password. It will be needed by any IRC client that tries
to connect to your server.

KICK - Eject a client from the channel
∗ INVITE - Invite a client to a channel
∗ TOPIC - Change or view the channel topic
∗ MODE - Change the channel’s mode:
· i: Set/remove Invite-only channel
· t: Set/remove the restrictions of the TOPIC command to channel
operators
· k: Set/remove the channel key (password)
· o: Give/take channel operator privilege
· l: Set/remove the user limit to channel

*/