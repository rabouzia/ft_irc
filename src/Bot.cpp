#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <signal.h>
#include <sstream>
#include <algorithm>

bool g_end = false;
int g_fd = -1;
std::vector<std::string> g_ban_words;
std::map<std::string, std::string> g_bot_responses;

void handle_quit(int sig)
{
	(void)sig;
	if (g_fd > 0)
		close(g_fd);
	g_end = true;
}

int main(int ac, char **av)
{
	if (ac != 4)
	{
		std::cerr << "Usage: ./bot <host> <port> <pass>" << std::endl;
		return (84);
	}
	std::cout << "---------------------------------------------------------\n";
	std::string host(av[1]);
	std::string port(av[2]);
	std::string pass(av[3]);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(std::atoi(port.c_str()));
	if (addr.sin_port == 0)
	{
		std::cerr << "Error: htons() failed" << std::endl;
		return (84);
	}
	if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) <= 0)
	{
		std::cerr << "Error: inet_pton() failed. Invalid IP address: " << host << std::endl;
		return (84);
	}
	g_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (g_fd < 0)
	{
		std::cerr << "Error: socket() failed" << std::endl;
		return (84);
	}
	if (connect(g_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		std::cerr << "Error: connect() failed" << std::endl;
		close(g_fd);
		return (84);
	}
	std::string msg("PASS " + pass + "\r\n");
	if (send(g_fd, msg.c_str(), msg.size(), 0) < 0)
	{
		std::cerr << "Error: send() failed" << std::endl;
		close(g_fd);
		return (84);
	}
	std::string name = "COPILOT";
	std::string msg2("NICK " + name + "\r\n");
	if (send(g_fd, msg2.c_str(), msg2.size(), 0) < 0)
	{
		std::cerr << "Error: send() failed" << std::endl;
		close(g_fd);
		return (84);
	}
	signal(SIGINT, handle_quit);
	while (!g_end)
	{
			std::string msg("BOT\r\n");
			send(g_fd, msg.c_str(), msg.size(), 0);
			sleep(300);  // 5 minutes = 300 seconds
	}
	return (0);
}
