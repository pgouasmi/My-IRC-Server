//
// Created by pgouasmi on 4/29/24.
//

#include "Kick.hpp"

Kick::Kick()
{
	this->_name = "JOIN";
	this->_permissionLevel = 1;
	this->_usage = "/kick #<channel> nickname";
	this->_description = "(+o) Kick a user from a channel";

	this->_expectedArgs.push_back(CHANNEL);
	this->_expectedArgs.push_back(USER);
}

static void kickReplyServer(Channel *channel, User *userToKick, std::string reason)
{
	std::vector<User *> vector = channel->getChannelsUsers();

	for (std::vector<User *>::iterator it = vector.begin() ; it != vector.end() ; ++it) {
		if ((*it) != userToKick)
			sendServerReply((*it)->getUserSocketFd(),
						RPL_KICK(user_id((*it)->getNickname(), (*it)->getUserName()), channel->getName(),
								 userToKick->getNickname(), reason), -1, DEFAULT);
	}
}

#include <iostream>

void Kick::execute(User *user, Channel *channel, std::vector<std::string> args)
{
	(void)user;

	std::string userNickname = args[1];
	std::string reason = "";
	IrcLogger *logger = IrcLogger::getLogger();
	User *userToKick;
	ChannelProperties *properties = channel->getProperties();

	if (args.size() > 2)
		reason = args[2];

	try {
		userToKick = channel->getUserByNickname(userNickname);
	}
	catch (const ChannelGetException &e)
	{
		logger->log(IrcLogger::ERROR, e.what());
		sendServerReply(user->getUserSocketFd(), ERR_USERNOTINCHANNEL(user->getNickname(), userNickname, channel->getName()), -1, DEFAULT);
		return ;
	}

	if (!properties->isUserOperator(user->getUniqueId()))
	{
		sendServerReply(user->getUserSocketFd(), ERR_CHANOPRIVSNEEDED(user->getNickname(), channel->getName()), -1, DEFAULT);
		return ;
	}
	channel->removeUserFromChannel(userToKick);
	sendServerReply(userToKick->getUserSocketFd(), RPL_NOTICE(userToKick->getNickname(), userToKick->getUserName(), user->getNickname(), reason), -1, DEFAULT);
	std::string reason2 = "you've been kicked out of #" + channel->getName();
	sendServerReply(userToKick->getUserSocketFd(), RPL_PART(user_id(userToKick->getNickname(), userToKick->getUserName()), channel->getName(), reason2), -1, DEFAULT);
	kickReplyServer(channel, userToKick, reason);
	channel->nameReplyAll();
}
