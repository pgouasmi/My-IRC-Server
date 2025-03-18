#include "Quit.hpp"

Quit::Quit()
{
	this->_name = "QUIT";
	this->_description = "Quit the server";
	this->_usage = "/quit <reason>";
	this->_permissionLevel = 0;
	this->_expectedArgs.push_back(STRING);
}

//static void sendQuitMessageToChannel(Channel *channel, User *leftUser, const std::string &message)
//{
//	std::vector<User *> userList = channel->getChannelsUsers();
//	if (userList.empty()) {
//		try {
//			ChannelCacheManager::getInstance()->deleteFromCache(channel->getUniqueId());
//			delete channel;
//		}
//		catch (ChannelCacheException &exception) {
//			IrcLogger *logger = IrcLogger::getLogger();
//			logger->log(IrcLogger::ERROR, exception.what());
//		}
//	}
//	for (std::vector<User *>::iterator it = userList.begin(); it != userList.end(); it++) {
//		sendServerReply((*it)->getUserSocketFd(),
//						RPL_QUIT(user_id(leftUser->getNickname(), leftUser->getUserName()), message),
//						-1, DEFAULT);
//	}
//}

void Quit::execute(User *user, Channel *channel, std::vector<std::string> args)
{
	(void) channel;
	try {
		std::vector<Channel *> channelList = user->getChannelList();
		if (channelList.empty()) {
			UsersCacheManager::getInstance()->deleteFromCache(user->getUniqueId());
			UsersCacheManager::getInstance()->addToLeftCache(user);
			return;
		}
		for (std::vector<Channel *>::iterator it = channelList.begin(); it != channelList.end(); it++) {
			(*it)->nameReplyAllExceptCaller(user->getNickname());
			(*it)->removeUserFromChannel(user);
//			sendQuitMessageToChannel(*it, user, StringUtils::getMessage(args));
			(*it)->quitReplyAll(user, StringUtils::getMessage(args));
		}

	}
	catch (UserCacheException &exception) {
		IrcLogger::getLogger()->log(IrcLogger::ERROR, "An error occurred during user quit command !");
		IrcLogger::getLogger()->log(IrcLogger::ERROR, exception.what());
	}
}