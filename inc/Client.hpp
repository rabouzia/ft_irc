/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdembele <mdembele@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 17:15:52 by rabouzia          #+#    #+#             */
/*   Updated: 2024/12/10 23:28:14 by mdembele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/types.h>  // Types de données de base
#include <sys/socket.h> // Fonctions de gestion de socket
#include <netinet/in.h> // Structures pour les adresses Internet
#include <arpa/inet.h>  // Conversion d'adresses et autres utilitaires réseaux
#include <netdb.h>      // Fonctions de résolution de noms
#include <unistd.h>     // Fonctions POSIX comme close()
#include <string>
#include <iostream>

class Server;

class Client{
	
	protected:
		// Server server;
		int Socket;
		sockaddr_in serverAddress;
		sockaddr_in clientAddress;
		bool psswdCheck;
		bool _InChannel;
		bool _nickVerified;
		bool _userNameVerified;
		

	public:
		Client(int FD, std::string nick)
		{
			psswdCheck = false;
			_InChannel = false;
			_nickVerified = false;
			_userNameVerified = false;
			Socket = FD;
			_old_buf = "";
			_nick =  nick;
		}
    	std::string _old_buf;
		~Client(){};
		std::string _nick;
		std::string _user;
		std::string _realname;
		std::string _mode;

		void Send();
		void Connect();
		std::string getNick() const 
		{
			return _nick;
		}
		void setNick(const std::string& nick)
		{
			_nick = nick;
		}

		void setNickVerified(bool status){
			_nickVerified = status;
		}
		bool getNickVerified() const{
			return _nickVerified;
		}
		std::string getUsername() const {return _user;}
		bool getUserNameVerified()  {return _userNameVerified;}
		void setUserNameVerified(bool status) {_userNameVerified = status;}
		void InChannel(bool status) {_InChannel = status;}
		int getSocket() const {return Socket;}
		bool getInChannel() const {return _InChannel;}
		bool GetPasswordVerified();
		void InitClient();
		void setPasswordVerified(bool status);

};
#endif
// std::string getHostname() {
// 	char hostname[1024];
// 	if (gethostname(hostname, sizeof(hostname)) == -1) {
// 		return "unknown";
// 	}
// 	return std::string(hostname);
// }
// std::string Client::getMask()t {
// 	std::string hostname = getHostname();
// 	return _nick + "!" + _user + "@" + hostname;
// }