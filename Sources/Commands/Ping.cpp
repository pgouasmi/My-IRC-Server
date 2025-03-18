/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:57:01 by jbadaire          #+#    #+#             */
/*   Updated: 2024/04/22 15:32:44 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ping.hpp"

#include <string>
#include <TimeUtils.hpp>
#include <iomanip>

Ping::Ping()
{
	this->_name = "Ping";
	this->_description = "Ping to check if alive";
	this->_usage = "/ping [username]";
	this->_permissionLevel = 0;
	this->_expectedArgs.push_back(STRING);
}

void Ping::execute(User *user, Channel *channel, std::vector<std::string> args)
{
		(void)channel;
		(void)user;
		(void)args;

		long currentTime = TimeUtils::getCurrentTimeMillis();
		user->setLastPingTimestamp(currentTime);

		std::string token = args[0];
		StringUtils::trim(token, "\r\n");
		sendServerReply(user->getUserSocketFd(), RPL_PONG(user_id(user->getNickname(), user->getUserName()), token), -1, DEFAULT);
}