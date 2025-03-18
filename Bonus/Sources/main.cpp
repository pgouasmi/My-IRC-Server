#include "../Includes/Bot.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 3) {
		std::cout << "Argument error. Usage: ./IRCBot <port> <password>" << std::endl;
		return 1;
	}
	try {
		Bot bot(argv[1], argv[2]);
		bot.botConnect();
		bot.botUp();
	}
	catch (BotBuildException &e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}