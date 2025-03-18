//
// Created by pgoua on 21/04/2024.
//

#include "Invite.hpp"

Invite::Invite()
{
	this->_name = "INVITE";
	this->_permissionLevel = 0;
	this->_usage = "/invite <nickname> #<channel>";
	this->_description = "Invite a user to join a channel";

	this->_expectedArgs.push_back(STRING);
	this->_expectedArgs.push_back(CHANNEL);
}

void Invite::execute(User *user, Channel *channel, std::vector<std::string> args)
{
	(void) channel;

	try {
		ChannelCacheManager *ChanManager = ChannelCacheManager::getInstance();
		UsersCacheManager *userManager = UsersCacheManager::getInstance();
		ChannelProperties *properties;
		Channel *channelP;
		User *invitedUserP;
		IrcLogger *logger = IrcLogger::getLogger();

		std::string invitedUser = args[0];
		std::string channelHash = args[1];

		if (channelHash[0] != '#')
		{
			logger->log(IrcLogger::ERROR, ("/invite: Error: Wrong usage:" + this->_usage));
			sendServerReply(user->getUserSocketFd(), ERR_ARG(user->getNickname(), this->_name, this->_usage), RED, BOLDR);
			return ;
		}

		std::string channelName = channelHash;
		StringUtils::trim(channelName, "#");

		try {
			channelP = ChanManager->getFromCacheString(channelName);
		}
		catch (ChannelCacheException &e)
		{
			logger->log(IrcLogger::ERROR, ("/invite: Error: " + user->getNickname() + "is not on channel " + channelName));
			sendServerReply(user->getUserSocketFd(), ERR_NOTONCHANNEL(user->getNickname(), channelName), RED, BOLDR);
			return ;
		}

		properties = channelP->getProperties();

		if (!userManager->doesNicknameAlreadyExist(invitedUser))
		{
			logger->log(IrcLogger::ERROR, ("/invite: Error: " + invitedUser + ": no such user"));
			sendServerReply(user->getUserSocketFd(), ERR_NOSUCHNICK(user->getNickname(), invitedUser), RED, BOLDR);
			return ;
		}
		if (!ChanManager->doesChannelExist(channelName))
		{
			logger->log(IrcLogger::ERROR, ("/invite: Error: " + channelName + ": no such channel"));
			sendServerReply(user->getUserSocketFd(), ERR_NOSUCHCHANNEL(user->getNickname(), channelName), RED, BOLDR);
			return ;
		}
		if (channelP->isUserInChannel(invitedUser))
		{
			logger->log(IrcLogger::ERROR, ("/invite: Error: " + invitedUser + " is already on channel " + channelName));
			sendServerReply(user->getUserSocketFd(), ERR_USERONCHANNEL(user->getNickname(), invitedUser, channelName), RED, BOLDR);
			return ;
		}
		invitedUserP = userManager->getFromNickname(invitedUser);
		sendServerReply(user->getUserSocketFd(), RPL_INVITING(user_id(user->getNickname(), user->getUserName()), user->getNickname(), invitedUser, channelName), -1, DEFAULT);
		sendServerReply(invitedUserP->getUserSocketFd(), RPL_INVITE(user_id(user->getNickname(), user->getUserName()), invitedUserP->getNickname(), channelName), -1, DEFAULT);

		properties->addToInvitedUsers(invitedUserP->getUniqueId());
	}
	catch (std::exception &e) {
		IrcLogger::getLogger()->log(IrcLogger::ERROR, e.what());
	}
}
