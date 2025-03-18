#include "Part.hpp"
#include <algorithm>

Part::Part()
{
	this->_name = "PART";
	this->_description = "Part a channel";
	this->_usage = "/part <#channel> <reason>";
	this->_permissionLevel = 0;
	this->_expectedArgs.push_back(CHANNEL);
	this->_expectedArgs.push_back(STRING);
}

void Part::sendPartMessageToChan(User *user, Channel *channel, const std::string &reason)
{
	std::vector<User *> userList = channel->getChannelsUsers();
	if (userList.empty())
	{
		try {
			ChannelCacheManager::getInstance()->deleteFromCache(channel->getUniqueId());
			delete channel;
		}
		catch (ChannelCacheException &exception) {
			IrcLogger *logger = IrcLogger::getLogger();
			logger->log(IrcLogger::ERROR, exception.what());
		}
		return;
	}
	else {
		for (std::vector<User *>::iterator it = userList.begin(); it != userList.end(); it++) {
			sendServerReply((*it)->getUserSocketFd(),
							RPL_PART(user_id(user->getNickname(), user->getUserName()), channel->getName(), reason), -1,
							DEFAULT);
		}
	}
}

void Part::execute(User *user, Channel *channel, std::vector<std::string> args)
{
	if (std::find_if(channel->getChannelsUsers().begin(), channel->getChannelsUsers().end(),
					 UserPredicateNickname(user->getNickname())) == channel->getChannelsUsers().end()) {
		sendServerReply(user->getUserSocketFd(), ERR_NOTONCHANNEL(user->getNickname(), channel->getName()), -1,
						DEFAULT);
		return;
	}

	try {
		channel = ChannelCacheManager::getInstance()->getFromCacheString(args.front().substr(1));
		args.erase(args.begin());
		std::string reason = StringUtils::getMessage(args);
		reason = " " + reason;
		sendServerReply(user->getUserSocketFd(), RPL_ENDOFNAMES(user->getNickname(), channel->getName()), -1, DEFAULT);
		sendServerReply(user->getUserSocketFd(),
						RPL_PART(user_id(user->getNickname(), user->getUserName()), channel->getName(), reason), -1,
						DEFAULT);
		channel->removeUserFromChannel(user);
		sendPartMessageToChan(user, channel, reason);
		channel->nameReplyAllExceptCaller(user->getNickname());
	}
	catch (ChannelCacheException &exception) {
		IrcLogger *logger = IrcLogger::getLogger();
		logger->log(IrcLogger::ERROR, exception.what());
	}
}