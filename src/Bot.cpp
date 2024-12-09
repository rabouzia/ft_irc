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

void parse_command(const std::string& line, std::string& prefix, std::string& command, std::vector<std::string>& parameters, std::string& suffix, bool& has_suffix)
{
	std::stringstream ss(line);
	std::string word;

	has_suffix = false;
	ss >> word;
	std::cout << word;
	unsigned long pos = word.find(":");
	while (pos == 0)
	{
		prefix.append(word);
		ss >> word;
		pos = word.find(":");
	}
	std::transform(word.begin(), word.end(), word.begin(), ::toupper);
	command = word;
	while (ss >> word && word[0] != ':')
	{
		parameters.push_back(word);
	}
	bool first = true;
	while (ss)
	{
		has_suffix = true;
		if (!first)
			suffix.append(" ");
		if (first && word[0] == ':')
			word.erase(0, 1);
		first = false;
		suffix.append(word);
		ss >> word;
	}
}
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}
int main(int ac, char **av)
{
	char buf[4096 + 1];
	int ret;
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
		return (84);
	}
	std::string msg("PASS " + pass + "\r\n");
	if (send(g_fd, msg.c_str(), msg.size(), 0) < 0)
	{
		std::cerr << "Error: send() failed" << std::endl;
		return (84);
	}
	std::string name = "COPILOT";
	std::string msg2("NICK " + name + "\r\n");
	if (send(g_fd, msg2.c_str(), msg2.size(), 0) < 0)
	{
		std::cerr << "Error: send() failed" << std::endl;
		return (84);
	}
	signal(SIGINT, handle_quit);
	while (!g_end)
	{
		// ret = read(g_fd, buf, 4096);
		// if (ret <= 0)
		// {
		// 	if (ret < 0)
		// 		std::cerr << "Error: read() failed" << std::endl;
		// 	return (84);
		// }
		ret = recv(g_fd, buf, sizeof(buf) - 1, 0);
		buf[ret] = 0;
		std::vector<std::string>data = split(buf, ' ');
		std::cout << buf << "\n";
		if (data[1] == "PRIVMSG")
		{
			std::string msg("PRIVMSG " + data[0] + " :Fuck You\r\n");
			std::cout << msg << "\n";
			send(g_fd, msg.c_str(), msg.size(), 0);
			sleep(500);
		}
	}
	return (0);
}
