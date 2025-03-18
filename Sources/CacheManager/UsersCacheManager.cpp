/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UsersCacheManager.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 14:35:07 by jbadaire          #+#    #+#             */
/*   Updated: 2024/04/23 19:34:10 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UsersCacheManager.hpp"
#include "UserExceptions.hpp"
#include <limits>
#include <algorithm>
#include <Configuration.hpp>
#include <iostream>
#include <IrcLogger.hpp>
#include <IRCPredicate.hpp>
#include <NumericReplies.hpp>
#include <TimeUtils.hpp>
#include <unistd.h>
#include "ChannelCacheManager.hpp"
#include "Server.hpp"

UsersCacheManager* UsersCacheManager::instance = NULL;

UsersCacheManager::UsersCacheManager() : users(std::list<User *>()), uniqueIdCounter(1)  {}

void UsersCacheManager::addToCache(User* user) throw (UserCacheException)
{
	if (this->uniqueIdCounter == std::numeric_limits<size_t>::max())
		throw UserCacheException(this->uniqueIdCounter, "Maximum number of users reached !");
	user->setUniqueId(++this->uniqueIdCounter);
	this->users.push_back(user);
}

void UsersCacheManager::addToLeftCache(User *user) throw (UserCacheException)
{
	this->leftUsers.push_back(user);
}

User* UsersCacheManager::getFromCache(size_t userId) throw (UserCacheException)
{
	std::list<User *>::iterator iterator = std::find_if(users.begin(), users.end(), UserPredicate(userId));
	if (iterator != users.end())
		return *iterator;
	throw UserCacheException(userId, "get user from id: User not found !");
}

User* UsersCacheManager::getFromCacheSocketFD(int socketFD) throw (UserCacheException)
{
	std::list<User *>::iterator iterator = std::find_if(users.begin(), users.end(), UserPredicateFD(socketFD));
	if (iterator != users.end())
		return *iterator;
	throw UserCacheException(socketFD, "get user from fd: User not found !");
}

User* UsersCacheManager::getFromNickname(std::string &nickname) throw (UserCacheExceptionString)
{
	std::list<User *>::iterator iterator = std::find_if(users.begin(), users.end(), UserPredicateNickname(nickname));
	if (iterator != users.end())
		return *iterator;
	throw UserCacheExceptionString(nickname, "get user from nickname: User not found !");
}

bool UsersCacheManager::doesNicknameAlreadyExist(const std::string &nickname) const
{
	return std::find_if(this->users.begin(), this->users.end(), UserPredicateNickname(nickname)) == this->users.end() ? false : true;
}

void UsersCacheManager::deleteTimeoutUsers(std::string serverName, Server &serv)
{
	size_t currentTimestamp = TimeUtils::getCurrentTimeMillis();
	std::list<User *> cachedUsers = getCache();
	std::vector<Channel *> channelList;

	if (!cachedUsers.size())
		return ;
	std::list<User *>::iterator users = cachedUsers.begin();
	size_t maxDifference = Configuration::getInstance()->getSection("SERVER")->getNumericValue("user_timeout", 78000);
	while (users != cachedUsers.end())
	{
		User *user = *users;
		if (user == NULL)
			continue;
		size_t total = user->getLastPingTimestamp() + maxDifference;
		if (currentTimestamp > total)
		{
			try
			{
				int userFd = user->getUserSocketFd();
				deleteFromCache(user->getUniqueId());
				addToLeftCache(user);
				sendServerReply(userFd, ERR_REQUESTTIMEOUT(StringUtils::ltos(userFd), serverName), RED,BOLDR);
				close(userFd);
				channelList = user->getChannelList();
				serv.removePollFd(userFd);
				for (std::vector<Channel *>::iterator it = channelList.begin() ; it != channelList.end() ; ++it) {
					(*it)->removeUserFromChannel(user);
					(*it)->quitReplyAll(user, "Timeout");
					(*it)->nameReplyAll();
				}
			}
			catch (UserCacheException &exception)
			{
				IrcLogger::getLogger()->log(IrcLogger::ERROR, "An error occurred during user timeout fixer !");
				IrcLogger::getLogger()->log(IrcLogger::ERROR, exception.what());

			}
		}
		++users;
	}
}

void UsersCacheManager::deleteFromCache(size_t userId) throw (UserCacheException)
{
	std::list<User *>::iterator iterator = std::find_if(users.begin(), users.end(),  UserPredicate(userId));
	if (iterator != users.end())
	{
		users.erase(iterator);
		return ;
	}
	throw UserCacheException(userId, "delete user from cache: User not found !");
}

std::list<User *> UsersCacheManager::getCache()
{
	return this->users;
}

size_t UsersCacheManager::getUniqueIdCounter() const
{
	return this->uniqueIdCounter;
}

UsersCacheManager* UsersCacheManager::getInstance()
{
	return instance == NULL ? instance = new UsersCacheManager() : instance;
}

UsersCacheManager::~UsersCacheManager()
{
	for (std::list< User *>::iterator it = this->users.begin() ; it != this->users.end() ; ++it)
		delete *it;
	for (std::list< User *>::iterator it = this->leftUsers.begin() ; it != this->leftUsers.end() ; ++it)
		delete *it;
}