
#include "UserProperties.hpp"
#include "User.hpp"
#include <algorithm>
#include "PrimitivePredicate.hpp"
UserProperties::UserProperties() : isAway(false) {}

void UserProperties::setAway(bool away)
{
	isAway = away;
}

bool UserProperties::getAway() const
{
	return isAway;
}

void UserProperties::addPermission(std::string permission)
{
	if (hasPermission(permission))
		return;
	permissions.push_back(permission);
}

void UserProperties::removePermission(std::string permission)
{
	permissions.remove_if(StringPredicate(permission));
}

bool UserProperties::hasPermission(const std::string& permission)

{
	return std::find_if(getPermissions().begin(), getPermissions().end(), StringPredicate(permission)) != getPermissions().end();
}

std::list<std::string> UserProperties::getPermissions()
{
	return permissions;
}

std::list<size_t> UserProperties::getJoinedChannels()
{
	return joinedChannels;
}

std::list<size_t> UserProperties::getNotifiedConnectionUsers()
{
	return notifiedConnectionUsers;
}

std::list<size_t> UserProperties::getBlockedUsers()
{
	return blockedUsers;
}

std::list<size_t> UserProperties::getIgnoredUsers()
{
	return ignoreUsers;
}