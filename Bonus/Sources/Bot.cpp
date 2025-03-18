#include "../Includes/Bot.hpp"
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>

bool Bot::botIsUp = false;

static size_t getTime()
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

static std::vector<std::string> split(const std::string &input, int c)
{
	std::vector<std::string> result;

	if (input.empty())
		return result;
	std::string trimmed = input;

	if (input.find(c) == std::string::npos)
	{
		result.push_back(input);
		return result;
	}

	size_t i = input.find(c);
	size_t j = 0;
	std::string line;

	while (input[i])
	{
		i = input.find(c, j);
		if (i == std::string::npos)
		{
			std::string subbed = input.substr(j, input.size() - j);
			result.push_back(subbed);
			break;
		}
		line = input.substr(j, i - j);
		i++;
		j = i;
		result.push_back(line);
		line.clear();
	}
	return result;
}

Bot::Bot(char *port, char *password) throw (BotBuildException)
{
	char *pEnd;
	double portValue = strtod(port, &pEnd);

	if (strlen(pEnd) != 0)
		throw (BotBuildException("Port must be digits only"));
	if (portValue < 1024 || portValue > 49151)
		throw (BotBuildException("Impossible port provided"));
	this->_port = static_cast<int>(portValue);

	this->_password = password;

	int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1)
		throw (BotBuildException("Error on socket creation"));

	memset(&this->_serverAddress, 0, sizeof(this->_serverAddress));
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
	this->_serverAddress.sin_port = htons(this->_port);

    this->_botPollFd.events = POLLIN;
    this->_botPollFd.fd = clientSocket;
	this->_lastPing = getTime();
}

void Bot::botConnect() throw(BotBuildException)
{
	if (connect(this->_botPollFd.fd, (struct sockaddr*)&this->_serverAddress, sizeof(this->_serverAddress)) == -1) {
		throw (BotBuildException("Error: connect() failed"));
	}
	std::string message = "CAP LS 302\nPASS " + this->_password + "\nNICK IRCHEH_BOT\nUSER IRCHEH_BOT 0 * :realname\r\n";
    if (send(this->_botPollFd.fd, message.c_str(), message.length(), 0) == -1)
		throw (BotBuildException("Error: send failed"));
	std::cout << "BOT IS UP. Ready to handle requests..." << std::endl;
}

static bool isRequestCorrect(const std::vector<std::string> &request) throw (BotBuildException)
{
	if (request[1] == "433")
		throw (BotBuildException("Error: Nickname 'IRCHEH_BOT' already taken. Exiting..."));
	if (request.size() != 4 || request[1] != "PRIVMSG" || request[0][0] != ':' || request[3][0] != ':')
		return false;
	return true;
}

void Bot::handleRequest(const std::string &request)
{
	std::vector<std::string> requestVector = split(request, ' ');

	if (isRequestCorrect(requestVector)) {
		std::string targetNick = requestVector[0].substr(1, requestVector[0].find('!', 1) - 1);
		std::cout << "Message received from " << targetNick << std::endl;
		std::string message = "PRIVMSG ";
		message += targetNick;
		message += " :I'm IRCHEH_BOT, I don't do much but I exist. Outstanding uwu ?\r\n";

		if (send(this->_botPollFd.fd, message.c_str(), 512, 0) == -1)
			std::cout << "err on send" << std::endl;
		std::cout << "Answer successfully sent to " << targetNick << std::endl;
	}
	else {
		throw BotRunException("Error: Incorrect request");
	}
}

static void signalHandler(int signum)
{
	std::cout << "\b\b  \b\b";
	if (signum == SIGINT)
		std::cout << "SIGINT received. Interrupting bot..." << std::endl;
	else if (signum == SIGQUIT)
		std::cout << "SIGQUIT received. Interrupting bot..." << std::endl;
	else if (signum == SIGTERM)
		std::cout << "SIGTERM received. Interrupting bot..." << std::endl;
	Bot::botIsUp = false;
}

void Bot::handleSignals()
{
	if (std::signal(SIGINT, signalHandler) == SIG_ERR)
	{
		std::cout << "Error in signals handling" << std::endl;
		botIsUp = false;
		return;
	}
	if (std::signal(SIGQUIT, signalHandler) == SIG_ERR)
	{
		std::cout << "Error in signals handling" << std::endl;
		botIsUp = false;
		return;
	}
	if (std::signal(SIGTERM, signalHandler) == SIG_ERR)
	{
		std::cout << "Error in signals handling" << std::endl;
		botIsUp = false;
	}
}

void Bot::botUp() throw(BotBuildException)
{
	botIsUp = true;
	handleSignals();
	while (botIsUp)
	{
		if (getTime() > this->_lastPing + 10000) {
			this->_lastPing = getTime();
			std::string pingMsg = "PING LAG7777\r\n";
			if (send(this->_botPollFd.fd, pingMsg.c_str(), pingMsg.length(), 0) == -1) {
				exit(1);
			}
		}
		int res = poll(&this->_botPollFd, 1, 100);
		if (res == -1 && errno != EINTR) {
			std::cout << "Error: Poll(). Exiting" << std::endl;
			botIsUp = false;
		}
		else if (this->_botPollFd.revents & POLLIN) {
			char buffer[512];
			bzero(buffer, 512);
			int size = recv(this->_botPollFd.fd, &buffer, 510, 0);
			if (size == -1) {
				close(this->_botPollFd.fd);
				exit(1);
			}
			if (size == 0) {
				exit(1);
			}
			else {
				std::string request = buffer;
				memset(buffer, 0, size);
				try {
					this->handleRequest(request);
				}
				catch (const BotRunException &e) {
					;
				}
				catch (const BotBuildException &e) {
					std::cout << e.what() << std::endl;
					return ;
				}
			}
		}
	}
	std::string quitMessage = "QUIT :Leaving\r\n";
	if (send(this->_botPollFd.fd, quitMessage.c_str(), quitMessage.length(), 0) == -1) {
		std::cout << "Error: send() failed" << std::endl;
	}
}