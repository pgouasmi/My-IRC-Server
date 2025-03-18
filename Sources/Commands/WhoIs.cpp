#include "WhoIs.hpp"

WhoIs::WhoIs()
{
	this->_name = "WHOIS";
	this->_description = "Get information about a user";
	this->_usage = "/whois <nickname>";
	this->_permissionLevel = 0;
	this->_expectedArgs.push_back(STRING);
}

void WhoIs::execute(User *user, Channel *channel, std::vector<std::string> args) {
	(void)channel;
	try {
		//TODO: CHECK IF WHOIS NEED MORE INFORMATIONS
		User *searchedUser = UsersCacheManager::getInstance()->getFromNickname(args.front());
		sendServerReply(user->getUserSocketFd(),
						RPL_WHOISUSER(searchedUser->getNickname(), searchedUser->getUserName(), "localhost", searchedUser->getRealName()),
						-1, DEFAULT);
		sendServerReply(user->getUserSocketFd(), RPL_ENDOFWHOIS(searchedUser->getNickname()), -1, DEFAULT);
	}
	catch (UserCacheExceptionString &exception)
	{
		IrcLogger *logger = IrcLogger::getLogger();
		logger->log(IrcLogger::ERROR, exception.what());
	}
}