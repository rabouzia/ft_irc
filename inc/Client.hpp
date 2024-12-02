/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rabouzia <rabouzia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:15:52 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/02 18:18:52 by rabouzia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>  // Types de données de base
#include <sys/socket.h> // Fonctions de gestion de socket
#include <netinet/in.h> // Structures pour les adresses Internet
#include <arpa/inet.h>  // Conversion d'adresses et autres utilitaires réseaux
#include <netdb.h>      // Fonctions de résolution de noms
#include <unistd.h>     // Fonctions POSIX comme close()
#include <iostream>

// chaque username est unique de len max 9

class Client{
	
	protected:
		std::string _username;
	
	public:
	int add_user(std::string name);
	bool is_inlist(std::string name);
};