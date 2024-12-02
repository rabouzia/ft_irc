/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabouzia <rabouzia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:44:33 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/02 18:34:42 by rabouzia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"


int KICK(std::string user, std::string channel);
		int INVITE(std::string user, std::string channel);
		int TOPIC(std::string user, std::string channel);


int Command::MODE(std::string user,char mode, std::string channel){
			
	if (mode == 'i' )
	{
		// Set/remove Invite-only channel
		return 1;
	}
	if (mode == 't' )
	{
		// t: Set/remove the restrictions of the TOPIC command to channel operators
		return 1;
		
	}
	if (mode == 'k' )
	{
		// k: Set/remove the channel key (password)
		return 1;
	}
	if (mode == 'o' )
	{
		// o: Give/take channel operator privilege
		return 1;
	}
	if (mode == 'l' )
	{
		// l: Set/remove the user limit to channel
		return 1;
	}
	else
	{
		// do no
		return 1;		
	}
}