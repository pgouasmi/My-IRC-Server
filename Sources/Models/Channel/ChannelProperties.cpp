#include "ChannelProperties.hpp"

ChannelProperties::ChannelProperties() : canalModes("t"), hasPassword(false), limitIsSet(false) {}

std::list<size_t> ChannelProperties::getUsersInChannel()
{
	return this->usersInChannel;
}

std::list<size_t> ChannelProperties::getMutedUsers()
{
	return this->mutedUsers;
}

std::list<size_t> ChannelProperties::getBannedUsers()
{
	return this->bannedUsers;
}

std::list<size_t> ChannelProperties::getInvited()
{
	return this->invited;
}

std::list<size_t> ChannelProperties::getOperators()
{
	return this->operators;
}

bool ChannelProperties::doesChannelHaveMode(char c) const
{
	if (this->canalModes.find(c) != std::string::npos)
		return true;
	return false;
}

#include <iostream>

bool ChannelProperties::doesUserHaveMode(size_t UserId, char c) const throw (ChannelGetException)
{
//	std::cout << "iin does user have mode" << std::endl;
	if (this->userModes.find(UserId) == userModes.end())
		throw (ChannelGetException("Error: No user with this ID"));
	if (this->userModes.find(UserId)->second.find(c) != std::string::npos)
		return true;
	return false;
}

void ChannelProperties::addModeToChannel(size_t callingUserId, char c) throw(ChannelGetException)
{
	if (!doesUserHaveMode(callingUserId, OPERATOR))
		throw (ChannelGetException("Error: User is not Operator"));
	if (doesChannelHaveMode(c))
		throw (ChannelGetException("Error: Channel already has this mode"));
	this->canalModes += c;
}

void ChannelProperties::removeModeToChannel(size_t callingUserId, char c) throw(ChannelGetException)
{
	if (!doesUserHaveMode(callingUserId, OPERATOR))
		throw (ChannelGetException("Error: User is not Operator"));
	if (!doesChannelHaveMode(c))
		throw (ChannelGetException("Error: Channel doesn't have this mode"));
	this->canalModes.erase(canalModes.find(c));
}

void ChannelProperties::addModeToUser(size_t targetId, size_t callingUserId, char c) throw(ChannelGetException)
{
	if (callingUserId != 0) {
		if (doesUserHaveMode(callingUserId, OPERATOR) == false)
			throw (ChannelGetException("Error: User is not Operator"));
	}
	if (doesUserHaveMode(targetId, c))
		throw (ChannelGetException("Error: User already has this mode"));

	this->userModes.find(targetId)->second += c;
}

void ChannelProperties::removeModeToUser(size_t targetId, size_t callingUserId, char c) throw(ChannelGetException)
{
	if (callingUserId != 0) {
		if (doesUserHaveMode(callingUserId, OPERATOR) == false)
			throw (ChannelGetException("Error: User is not Operator"));
	}
	if (!doesUserHaveMode(targetId, c))
		throw (ChannelGetException("Error: User doesn't have this mode"));

	this->userModes[targetId].erase(this->userModes[targetId].find(c));
}

void ChannelProperties::addUserToChannel(size_t userId)
{
	std::pair<size_t, std::string> newUser;
	newUser.first = userId;
	newUser.second = "";

	this->userModes.insert(newUser);
}

std::string &ChannelProperties::getPassword()
{
	return this->password;
}

void ChannelProperties::setPassword(const std::string &newPassword)
{
	this->password = newPassword;
}

bool ChannelProperties::isPasswordSet() const
{
	return this->hasPassword;
}

void ChannelProperties::setPasswordStatus(bool value)
{
	this->hasPassword = value;
}

std::string &ChannelProperties::getTopic()
{
	return this->topic;
}

#include <iostream>

void ChannelProperties::setTopic(size_t userId, const std::string &newTopic)
{
	if (doesUserHaveMode(userId, OPERATOR))
		this->topic = newTopic;
}

int ChannelProperties::getUserLimit() const
{
	return this->userLimit;
}

void ChannelProperties::setUserLimit(int limit)
{
	this->userLimit = limit;
}

bool ChannelProperties::getUserLimitStatus() const
{
	return this->limitIsSet;
}

void ChannelProperties::setUserLimitStatus(bool value)
{
	this->limitIsSet = value;
}

int ChannelProperties::getNumberOfUsers() const
{
	return this->userModes.size();
}

bool ChannelProperties::isChannelFull() const
{
	if (this->getUserLimitStatus() == false)
		return false;
	return (getNumberOfUsers() >= this->userLimit ? true : false);
}

bool ChannelProperties::isUserOperator(size_t userId) const
{
	return this->doesUserHaveMode(userId, 'o');
}

bool ChannelProperties::isUserInvited(size_t userId) const
{
	return (std::find(this->invited.begin(), this->invited.end(), userId) != this->invited.end());
}

void ChannelProperties::addToInvitedUsers(size_t userId)
{
	if (this->isUserInvited(userId))
		return ;
	this->invited.push_back(userId);
}

bool ChannelProperties::isUserOnChannel(size_t userId) const
{
	return (this->userModes.find(userId) != userModes.end());
}

std::string &ChannelProperties::getUserModes(size_t userId) throw (ChannelGetException)
{
	if (this->isUserOnChannel(userId))
		return (this->userModes[userId]);
	throw (ChannelGetException("Error: User is not Operator"));
}

std::string ChannelProperties::getChannelModes() const
{
	return this->canalModes;
}

std::map<size_t, std::string> &ChannelProperties::getMap()
{
	return this->userModes;
}

void ChannelProperties::removeUserFromChannel(size_t userId)
{
	if (this->userModes.find(userId) != userModes.end())
		userModes.erase(userId);
}

void ChannelProperties::addUserToBannedUsers(size_t userId)
{
	std::list<size_t >::iterator it = std::find(this->bannedUsers.begin(), this->bannedUsers.end(), userId);

	if (it == this->bannedUsers.end())
		this->bannedUsers.push_back(userId);
}

void ChannelProperties::removeUserFromBannedUsers(size_t userId)
{
	std::list<size_t >::iterator it = std::find(this->bannedUsers.begin(), this->bannedUsers.end(), userId);

	if (it != this->bannedUsers.end())
		this->bannedUsers.erase(it);
}

bool ChannelProperties::isUserBanned(size_t userId) const
{
	std::list<size_t >::const_iterator it = std::find(this->bannedUsers.begin(), this->bannedUsers.end(), userId);

	return (it != this->bannedUsers.end());
}