//
// Created by pgouasmi on 4/16/24.
//

#include "Join.hpp"

Join::Join()
{
	this->_name = "JOIN";
	this->_permissionLevel = 0;
	this->_usage = "/join #<channel> <password>";
	this->_description = "Used to join an existing channel, or create a new one if no channel corresponds to the given channel name.";

	this->_expectedArgs.push_back(STRING);
}

#include <iostream>
/*
 * [1] Create a vector of pairs channelName/password
 * [2] Iterate through this vector, if the channel does not exist --> [3], else --> [4]
 *
 * [3] Create a new channel and add it to the channel cache manager
 *
 * [4] try to make the user join an existing channel
 * 		- if the user is already on the channel --> return
 * 		- check if a password is set and compare it to the one given to the client
 * 		*/

//TODO: FIX LE SEND DU TOPIC (BACK)
void Join::execute(User *user, Channel *channel, std::vector<std::string>args)
{
	(void)user;
	(void)channel;
	std::vector<std::pair<std::string, std::string> > ChannelsPasswords;
	std::vector<std::string> Channels = StringUtils::split(args.front(), ',');
	std::vector<std::string> Passwords;
	ChannelProperties *properties;

	if (args.size() > 1) {
		Passwords = StringUtils::split(args[1], ',');
		for (size_t i = Passwords.size() ; i < Channels.size() ; i++)
			Passwords.push_back("");
	}
	else
	{
		for (size_t i = 0 ; i < Channels.size() ; i++)
			Passwords.push_back("");
	}

	std::vector<std::string>::iterator passIt = Passwords.begin();

	//[1]
	for (std::vector<std::string>::iterator chanIt = Channels.begin() ; chanIt != Channels.end() ; ++chanIt)
	{
		std::pair<std::string, std::string> result;
		result.first = *chanIt;
		result.second = *passIt;

		ChannelsPasswords.push_back(result);

		passIt++;
	}
	//[1]

	ChannelCacheManager *ChanManager = ChannelCacheManager::getInstance();
	ChannelBuilder Builder;

	//[2]
	//check the channel cache manager, if !exist->add new, else to implement(join existing channel with pword if necessary)
	for (std::vector<std::pair<std::string, std::string> >::iterator it = ChannelsPasswords.begin() ; it != ChannelsPasswords.end() ; ++it)
	{
		std::string channelName = it->first;
		StringUtils::trim(channelName, "#");
		if (it->first[0] != '#')
			sendServerReply(user->getUserSocketFd(), RPL_NOTICE(user->getNickname(), user->getUserName(), "/join", "needs '#' before channel name"), -1, DEFAULT);
		else if (ChanManager->doesChannelExist(channelName))
		{
			//[4]
			try {
				Channel *existingChannel = ChanManager->getFromCacheString(channelName);
				properties = existingChannel->getProperties();
				if (properties->isChannelFull()) {
					sendServerReply(user->getUserSocketFd(), ERR_CHANNELISFULL(user->getNickname(), channelName), -1, DEFAULT);
				}
				else if (existingChannel->isUserInChannel(user->getNickname())) {
					sendServerReply(user->getUserSocketFd(),
								ERR_USERONCHANNEL(user->getNickname(), user->getUserName(), existingChannel->getName()),
								RED, DEFAULT);
				}
				else if (properties->isPasswordSet() == true && it->second != properties->getPassword()) {
				sendServerReply(user->getUserSocketFd(),
								ERR_BADCHANNELKEY(user->getNickname(), existingChannel->getName()), -1, DEFAULT);
				}
				else if (properties->doesChannelHaveMode('i') && !properties->isUserInvited(user->getUniqueId()))
				{
						sendServerReply(user->getUserSocketFd(), ERR_INVITEONLYCHAN(user->getNickname(), existingChannel->getName()), -1, DEFAULT);
				}
				else if (properties->isUserBanned(user->getUniqueId()))
				{
					sendServerReply(user->getUserSocketFd(), ERR_BANNEDFROMCHAN(user->getNickname(), existingChannel->getName()), -1, DEFAULT);
				}
				else {
					user->addChannelToList(existingChannel);
					properties->addUserToChannel(user->getUniqueId());
					existingChannel->addUserToChannel(user);
					sendServerReply(user->getUserSocketFd(), RPL_JOIN(user_id(user->getNickname(), user->getUserName()),
																	  existingChannel->getName()), -1, DEFAULT);
					if (!existingChannel->getTopic().empty())
						sendServerReply(user->getUserSocketFd(),
										RPL_TOPIC(user->getNickname(), existingChannel->getName(),
												  properties->getTopic()), GREEN, BOLDR);
					else
						sendServerReply(user->getUserSocketFd(),
										RPL_NOTOPIC(user->getNickname(), existingChannel->getName()),
										GREEN, DEFAULT);

					existingChannel->nameReplyAll();
				}

			}
			catch (ChannelCacheException &e)
			{
				IrcLogger *logger = IrcLogger::getLogger();
				logger->log(IrcLogger::ERROR, e.what());
				return ;
			}
			//[4]
		}
		else
		{
			//[3]
			//create new channel
			Builder.setName(channelName);
			Builder.setPassword(it->second);
			Builder.setTopic("");
			Channel *newChannel;
			try {
				newChannel = Builder.build();
				newChannel->addUserToChannel(user);
				ChanManager->addToCache(newChannel);
			}
			catch (std::exception &e)
			{
				IrcLogger *logger = IrcLogger::getLogger();
				logger->log(IrcLogger::ERROR, e.what());
				return ;
			}

			properties = newChannel->getProperties();
			properties->addUserToChannel(user->getUniqueId());
			properties->addModeToUser(user->getUniqueId(), 0, OPERATOR);
			properties->setPasswordStatus(false);
			user->addChannelToList(newChannel);
			sendServerReply(user->getUserSocketFd(), RPL_JOIN(user_id(user->getNickname(), user->getUserName()), newChannel->getName()), -1, DEFAULT);
			sendServerReply(user->getUserSocketFd(), RPL_NOTOPIC(user->getUserName(), ChanManager->getCache().front()->getName()), -1, DEFAULT);
			sendServerReply(user->getUserSocketFd(), RPL_NAMREPLY(user->getNickname(), "<@|*=|:|>", newChannel->getName(),newChannel->getUserList()), -1, DEFAULT);
			//[3]
			// std::cout << "users on channel: " << newChannel->getChannelsUsers().size() << std::endl;
		}
	}
	//[2]
}
