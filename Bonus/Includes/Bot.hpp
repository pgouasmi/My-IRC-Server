//
// Created by pgoua on 08/05/2024.
//

#ifndef BOT_HPP
#define BOT_HPP

#include <string>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <time.h>
#include <csignal>
#include <cerrno>

#include "BotBuildException.hpp"
#include "BotRunException.hpp"

class Bot
{
private:
	std::string	_message;
	int 		_port;
	std::string _password;
	sockaddr_in _serverAddress;
	pollfd		_botPollFd;
	size_t		_lastPing;

public:
	Bot(char *port, char *password) throw (BotBuildException);
	void botConnect() throw (BotBuildException);
	void botUp() throw (BotBuildException);

	std::string getMessage() const;
	void setMessage(const std::string &data);
	void handleRequest(const std::string &request);
	static void handleSignals();
	static bool botIsUp;
};

#endif
