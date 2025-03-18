#include "Server.hpp"
#include "Colors.hpp"
#include "ChannelCacheManager.hpp"
#include "CommandManager.hpp"

int main(int argc, char **argv)
{
	std::string str =
	"			 /$$$$$$ /$$$$$$$   /$$$$$$                       /$$$$$$  /$$$$$$$$ /$$$$$$$  /$$    /$$ /$$$$$$$$ /$$$$$$$\n"
	"			|_  $$_/| $$__  $$ /$$__  $$                     /$$__  $$| $$_____/| $$__  $$| $$   | $$| $$_____/| $$__  $$\n"
	"			  | $$  | $$  \\ $$| $$  \\__/                    | $$  \\__/| $$      | $$  \\ $$| $$   | $$| $$      | $$  \\ $$\n"
	"			  | $$  | $$$$$$$/| $$             /$$$$$$      |  $$$$$$ | $$$$$   | $$$$$$$/|  $$ / $$/| $$$$$   | $$$$$$$/\n"
	"			  | $$  | $$__  $$| $$            |______/       \\____  $$| $$__/   | $$__  $$ \\  $$ $$/ | $$__/   | $$__  $$\n"
	"			  | $$  | $$  \\ $$| $$    $$                     /$$  \\ $$| $$      | $$  \\ $$  \\  $$$/  | $$      | $$  \\ $$\n"
	"			 /$$$$$$| $$  | $$|  $$$$$$/                    |  $$$$$$/| $$$$$$$$| $$  | $$   \\  $/   | $$$$$$$$| $$  | $$\n"
	"			|______/|__/  |__/ \\______/                      \\______/ |________/|__/  |__/    \\_/    |________/|__/  |__/\n"
	"\n							Pgouasmi, Chonorat, Jbadaire																	\n";

	Colors bold(Colors::BOLD);
	Colors blue(Colors::FG_BLUE);
	Colors reset(Colors::RESET);
	std::cout << bold << blue << str << reset << std::endl;

	std::cout << "Loading, IRCLogger..." << std::endl;
	IrcLogger *logger = IrcLogger::getLogger();
	logger->open("irc_server.log");
	logger->log(IrcLogger::TRACE, "The logger is up !");

	if (argc != 3)
	{
		logger->log(IrcLogger::ERROR, "Usage: ./ircserv <port> <password>");
		return (delete logger, 1);
	}

	logger->log(IrcLogger::TRACE, "Loading, IRCConfiguration...");
	Configuration *configuration = Configuration::getInstance("config.conf");
	try
	{
		configuration->load();
		logger->log(IrcLogger::INFO, "The configuration has been loaded !");
		logger->log(IrcLogger::INFO, "Redefine logger level from configuration");
		ConfigurationSection *serverSection = configuration->getSection("SERVER");
		if (serverSection == NULL)
		{
			logger->log(IrcLogger::ERROR, "Section \"SERVER\" not found in configuration file !");
			return (delete logger, delete configuration, 1) ;
		}
		std::string strLevel = serverSection->getStringValue("log_level", "TRACE");
		IrcLogger::LogLevel level = logger->getLogLevelFromString(strLevel);
		logger->setLogLevel(level);
		logger->log(IrcLogger::INFO, "Logger level has been redefined !");
		logger->log(IrcLogger::INFO, "Replace port and password inside configuration");
		serverSection->addEntry("port", argv[1]);
		serverSection->addEntry("password", argv[2]);
		logger->log(IrcLogger::INFO, "Port and password has been replaced !");
		bool printConfig = serverSection->getBooleanValue("print_config", false);
		std::string printConfigStr = printConfig ? "true" : "false";
		logger->log(IrcLogger::TRACE, "Printing configuration : " + printConfigStr);
		if (printConfig)
			configuration->printSections();
		try
		{
			logger->log(IrcLogger::INFO, "Starting server...");
			Server server;
			try
			{
				server.serverUp();
			}
			catch (ServerStartingException &exception)
			{
				logger->log(IrcLogger::ERROR, "An error occurred during server starting !");
				logger->log(IrcLogger::ERROR, exception.what());
			}
		}
		catch (ServerInitializationException &exception)
		{
			logger->log(IrcLogger::ERROR, "An error occurred during server initialization !");
			logger->log(IrcLogger::ERROR, exception.what());
		}
	}
	catch (ConfigurationIOException &exception)
	{
		logger->log(IrcLogger::ERROR, "An error occurred during configuration loading !");
		logger->log(IrcLogger::ERROR, exception.what());
	}
	delete configuration;
	delete logger;
	delete ChannelCacheManager::getInstance();
	delete CommandManager::getInstance();
	delete UsersCacheManager::getInstance();
	return 0;
}
