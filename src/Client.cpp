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


void Client::InitClient()
{

}

void Client::Connect()
{
	
}

void Client::setPasswordVerified(bool status)
{
	psswdCheck = status;
}

bool Client::GetPasswordVerified()
{
	return psswdCheck;
}
// void Client::ValidUser()
// {
// 	/*Chaque utilisateur n'a pas plus de neuf (9) caractères. Caractères disponibles dans le surnom:
// 	 a-z, A-Z, 0-9 et symboles spéciaux -, [, ], \, ^, {, }*/
// }

void Client::Send()
{

}

/*
int Client::Ping (std::string msg){

	
	send(_fd, (msg + "\r\n").c_str(), msg.length() + 2, MSG_NOSIGNAL);
	return 1;
}


*/