#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include "Channel.hpp"
#include "Client.hpp"

// Pré-déclaration des classes pour éviter les inclusions circulaires
class Client;
class Channel;

class Server {
private:
    // Attributs principaux
    int serverSocket;
    sockaddr_in serverAddress;
    int epollFd;
    int num;
    std::string _passwd;
    std::string serverName;
    std::string _old_buf;
    // Maps pour gérer les clients et les channels
    std::map<int, Client*> clientImap;                // Associe un FD à un client
    std::map<std::string, Client*> clientSmap;        // Associe un pseudo à un client
    std::map<std::string, Channel*> channelSmap;      // Associe un nom de channel à un Channel

    // Méthodes privées pour le fonctionnement interne
    void setNonBlocking(int fd);
    void handleNewConnection();
    void handleClientMessage(int clientFd);
    void disconnectClient(int fd);
    void sendToClient(int fd, const std::string& response);
    void parsingData(std::string& str, int clientFD);

    // Gestion des commandes IRC
    void handlePartCommand(const std::vector<std::string>& data, int ClientFD);
    void handleKickCommand(const std::vector<std::string>& data, int ClientFD);
    void handlePassCommand(const std::vector<std::string>& data, int ClientFD);
    void handleNickCommand(const std::vector<std::string>& data, int ClientFD);
    void handlePingCommand(const std::vector<std::string>& data, int ClientFD);
    void handlePrivmsgCommand( std::vector<std::string>& data, int ClientFD);
    void handleJoinCommand(const std::vector<std::string>& data, int ClientFD);
    void handleModeCommand(const std::vector<std::string>& data, int ClientFD);
    void handleInviteCommand(const std::vector<std::string>& data, int ClientFD);
    void handleTopicCommand(const std::vector<std::string>& data, int ClientFD);
	void handleUserCommand(std::vector<std::string> &data, int ClientFD);
	void handleBotCommand( int ClientFD);
	void handleQuitCommand(int ClientFD);
    // Helper pour les commandes MODE
    void handleModeChange(Channel *channel, char mode, bool addingMode, const std::vector<std::string>& data, int ClientFD);

public:
    // Constructeur et destructeur
    Server(int port, const std::string &passwd);
    ~Server();

    // Méthodes principales
    void initServer();
    void bindServ();
    void listenServ();
    void run();

    // Méthodes utilitaires
	void SendRPL(int clientSocket, const std::string &replyCode, const std::string &nickname, const std::string &message);
    int isAClient(const std::string name)
	{
		if (clientSmap[name] && (clientSmap[name]->getNickVerified() == true) && (clientSmap[name]->getUserNameVerified() == true) && (clientSmap[name]->GetPasswordVerified() == true))	
			return 1;
		return -1;
	}
    bool isChannel(const std::string& channel)
	{
		if (channelSmap[channel])
			return true;
		return false;
	}
};

std::vector<std::string> split (const std::string &s, char delim);
#endif // SERVER_HPP
