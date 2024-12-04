/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramzerk <ramzerk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 09:14:41 by ramzerk           #+#    #+#             */
/*   Updated: 2024/12/04 09:15:15 by ramzerk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

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