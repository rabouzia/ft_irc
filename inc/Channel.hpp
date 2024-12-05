/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabouzia <rabouzia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:13:48 by rabouzia          #+#    #+#             */
/*   Updated: 2024/11/23 17:14:25 by rabouzia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#pragma once

#include "Client.hpp"
#include <map>
#include <string>
class Client;
class Channel{

	protected:
		std::map<std::string, Client> ClientMap;
		std::map<std::string, Client> OperatorMap;

	public:	

};