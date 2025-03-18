/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 14:48:17 by jbadaire          #+#    #+#             */
/*   Updated: 2024/04/24 18:17:57 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"

#include <Server.hpp>

Nick::Nick()
{
	this->_name = "NICK";
	this->_description = "Change your nickname";
	this->_usage = "/Nick <new nickname>";
	this->_permissionLevel = 0;
	this->_expectedArgs.push_back(STRING);
}

void Nick::execute(User *user, Channel *channel, std::vector<std::string> args)
{
	(void)user;
	(void)args;
	(void)channel;

	std::string newNick = args[0];
	IrcLogger::getLogger()->log(IrcLogger::DEBUG, "New nickname: " + newNick);
	bool newNickIsOk = true;
	try
	{
		UsersCacheManager::getInstance()->getFromNickname(newNick);
		newNickIsOk = false;
	}
	catch (UserCacheExceptionString &ignored)
	{}

	//Check if the nickname is banned from the channel
	if (newNickIsOk)
	{
		std::vector<Channel *> inChannels = user->getChannelList();
		for (std::vector<Channel *>::iterator it = inChannels.begin(); it != inChannels.end(); it++)
		{
			Channel *channel = *it;
			try {
				if (channel->getProperties()->doesUserHaveMode(user->getUniqueId(), 'b')) {
					newNickIsOk = false;
					break;
				}
			}
			catch (ChannelGetException &ignored)
			{}
		}
	}

	//Check if the nickname is censorred
	if (newNickIsOk && StringUtils::hasCensuredWord(newNick, Configuration::getInstance()->getCensoredWords()).first)
		newNickIsOk = false;

	if (!newNickIsOk)
	{
		std::string serverName = Configuration::getInstance()->getSection("SERVER")->getStringValue("name", "IRCServer");
		sendServerReply(user->getUserSocketFd(), ERR_NICKNAMEINUSE(user->getUserName(), user->getNickname()), RED, BOLDR);
		return;
	}

	sendServerReply(user->getUserSocketFd(), RPL_NICK(user->getNickname(), user->getUserName(), newNick), -1, DEFAULT);
	user->setNickname(newNick);
	std::vector<Channel *> inChannels = user->getChannelList();
	for (std::vector<Channel *>::iterator it = inChannels.begin(); it != inChannels.end(); it++)
	{
		(*it)->nameReplyAll();
	}
}
