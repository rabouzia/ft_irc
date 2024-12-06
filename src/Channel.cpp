/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdmessa <abdmessa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:44:37 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/07 00:15:57 by abdmessa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, std::string password, Client *client) : _name(name), _password(password) {
	ClientMap[client->getSocket()] = client;
	OperatorMap[client->getSocket()] = client;
	client->InChannel(true);
	this->_topicAllow = true; // on le met a true par default
	
}


// void Channel::modeFunction(int flag) {

// }
