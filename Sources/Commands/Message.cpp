/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 18:11:56 by jbadaire          #+#    #+#             */
/*   Updated: 2024/04/24 18:11:58 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by pgouasmi on 4/18/24.
//

#include "Message.hpp"
#include "IrcLogger.hpp"

Message::Message()
{
	this->_name = "PRIVMSG";
	this->_permissionLevel = 0;
	this->_usage = "/msg <recipient> <message>";
	this->_description = "Sends a private message to a chosen user.";

	this->_expectedArgs.push_back(USER);
	this->_expectedArgs.push_back(STRING);
}

void Message::execute(User *user, Channel *channel, std::vector<std::string> args)
{
	(void)user;
	(void)channel;
	(void)args;

	std::string recipient = args.front();
	args.erase(args.begin());

	std::string message;

	for (std::vector<std::string>::iterator it = args.begin() ; it != args.end() ; ++it)
	{
		message += *it;
		message += " ";
	}
	StringUtils::trim(message, " ");

	if (recipient[0] == '#') //msg on a channel
	{
//		StringUtils::trim(recipient, "#");
		std::string channelName = recipient;
		StringUtils::trim(channelName, "#");

		try {
			Channel *currentChannel = ChannelCacheManager::getInstance()->getFromCacheString(channelName);
			ChannelProperties *chanProp = currentChannel->getProperties();
			if (!currentChannel->isUserInChannel(user->getNickname())) {
				sendServerReply(user->getUserSocketFd(), ERR_CANNOTSENDTOCHAN(user->getNickname(), channelName), -1, DEFAULT);
				return ;
			}
			if (chanProp->doesUserHaveMode(user->getUniqueId(), 'q') == true) {
				sendServerReply(user->getUserSocketFd(), ERR_BANNEDFROMCHAN(user->getNickname(), currentChannel->getName()), -1, DEFAULT);
				return ;
			}
			std::vector < User * > usersInChannel = currentChannel->getChannelsUsers();
			for (std::vector<User *>::iterator it = usersInChannel.begin(); it != usersInChannel.end(); ++it) {
				if ((*it)->getNickname() != user->getNickname())
					sendServerReply((*it)->getUserSocketFd(),
									RPL_PRIVMSG(user->getNickname(), user->getUserName(), recipient, message), -1,
									DEFAULT);
			}
		}
		catch (ChannelCacheException &e)
		{
			sendServerReply(user->getUserSocketFd(), ERR_CANNOTSENDTOCHAN(user->getNickname(), channelName), -1, DEFAULT);
			IrcLogger *logger = IrcLogger::getLogger();
			logger->log(IrcLogger::ERROR, e.what());
			return ;
		}
	}
	else //private message
	{
		std::vector<std::string> recipients = StringUtils::split(recipient, ',');
		for (std::vector<std::string>::iterator it = recipients.begin(); it != recipients.end(); ++it) {
			recipient = *it;
			StringUtils::trim(recipient, ",");
			try {
				User *Recipient = UsersCacheManager::getInstance()->getFromNickname(recipient);
				sendServerReply(Recipient->getUserSocketFd(),
								RPL_PRIVMSG(user->getNickname(), user->getUserName(), recipient, message), -1, DEFAULT);
				// sendServerReply(user->getUserSocketFd(),
				// 				RPL_PRIVMSG(user->getNickname(), user->getUserName(), recipient, message), -1, DEFAULT);
			}
			catch (UserCacheExceptionString &exception) {
				sendServerReply(user->getUserSocketFd(), ERR_NOSUCHNICK(user->getNickname(), recipient), -1, DEFAULT);
				IrcLogger *logger = IrcLogger::getLogger();
				logger->log(IrcLogger::ERROR, "An error occurred during message sending !");
				logger->log(IrcLogger::ERROR, exception.what());
				std::string tmp = "Nickname: ";
				logger->log(IrcLogger::ERROR, tmp.append(exception.getValue()));
			}
		}
	}
}

