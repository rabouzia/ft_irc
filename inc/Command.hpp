/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabouzia <rabouzia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:16:16 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/02 18:21:10 by rabouzia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

class Command{
	
	protected:

	public:
		int KICK(std::string user, std::string channel);
		int INVITE(std::string user, std::string channel);
		int TOPIC(std::string user, std::string channel);
		int MODE(std::string user,char mode, std::string channel);
};