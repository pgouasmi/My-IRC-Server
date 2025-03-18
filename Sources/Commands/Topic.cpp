//
// Created by pgouasmi on 4/25/24.
//

#include "Topic.hpp"

Topic::Topic()
{
	this->_name = "TOPIC";
	this->_permissionLevel = 1;
	this->_usage = "/topic #<channel> New Topic";
	this->_description = "Modify channel's topic";

	this->_expectedArgs.push_back(CHANNEL);
}

bool isTopicValid(std::string topic)
{
	if (topic.length() > 100 || !StringUtils::isPrintable(topic))
	{
		return false;
	}
	return true;
}

void Topic::execute(User *user, Channel *channel, std::vector<std::string> args)
{
	// std::cout << "Topic command" << std::endl;
	(void)channel;
	if (args.size() < 1)
	{
		sendServerReply(user->getUserSocketFd(), ERR_NEEDMOREPARAMS(user->getNickname(), this->_name), -1, DEFAULT);
		return ;
	}

	std::string channelName = args[0];
	StringUtils::trim(channelName, "#");

	Channel *currentChannel = ChannelCacheManager::getInstance()->getFromCacheString(channelName);
	ChannelProperties *properties = currentChannel->getProperties();


	if (args.size() == 1)
	{

		if (!properties->getTopic().empty())
			sendServerReply(user->getUserSocketFd(), RPL_TOPIC(user->getNickname(), currentChannel->getName(), properties->getTopic()), -1, DEFAULT);
		else
			sendServerReply(user->getUserSocketFd(), RPL_NOTOPIC(user->getNickname(), currentChannel->getName()), -1, DEFAULT);
	}
	else
	{
		if (properties->doesChannelHaveMode('t') && !properties->isUserOperator(user->getUniqueId()))
		{
			sendServerReply(user->getUserSocketFd(), ERR_CHANOPRIVSNEEDED(user->getNickname(), currentChannel->getName()), -1, DEFAULT);
			return ;
		}
		std::string res;
		for (std::vector<std::string>::iterator it = (args.begin()) ; it != args.end() ; ++it)
		{
			if (it != args.begin()) {
				res += *it;
				if (it == ++args.begin())
					StringUtils::trim(res, ": ");
				res += " ";
			}
		}
		StringUtils::trim(res, " ");
		if (!isTopicValid(res))
		{
			std::string reply = RPL_PRIVMSG(user->getNickname(), user->getUserName(), user->getNickname(), "Topic is invalid");
    		sendServerReply(user->getUserSocketFd(), reply, 0, DEFAULT);
			return ;
		}
		try {
			properties->setTopic(user->getUniqueId(), res);
			channel->setTopic(res);
			channel->topicReplyAll();
		}
		catch (std::exception &e) {
			IrcLogger::getLogger()->log(IrcLogger::ERROR, e.what());
		}
		// sendServerReply(user->getUserSocketFd(), RPL_TOPIC(user->getNickname(), currentChannel->getName(), properties->getTopic()), -1, DEFAULT);
	}
}

