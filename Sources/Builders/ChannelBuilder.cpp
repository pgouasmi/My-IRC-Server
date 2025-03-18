#include "ChannelBuilder.hpp"

#include <StringUtils.hpp>
#include <ChannelExceptions.hpp>
#include "Channel.hpp"
#include "IrcLogger.hpp"

ChannelBuilder& ChannelBuilder::setName(const std::string& name) {
	this->name = name;
	return *this;
}

ChannelBuilder& ChannelBuilder::setTopic(const std::string& topic) {
	this->topic = topic;
	return *this;
}

ChannelBuilder& ChannelBuilder::setPassword(const std::string& password) {
	this->password = password;
	return *this;
}

ChannelBuilder& ChannelBuilder::setProperties(const ChannelProperties& properties)
{
	this->properties = properties;
	return *this;
}

static bool isChannelNameValid(bool authorizeEmpty, std::string str) {
	if ((!authorizeEmpty && str.empty()) || str.length() > 100) {
		IrcLogger::getLogger()->log(IrcLogger::ERROR, "ChannelBuilder: Channel name is empty or too long");
		return false;
	}
	if ((!authorizeEmpty && StringUtils::isOnlyWhitespace(str)) || (!authorizeEmpty && !StringUtils::isPrintable(str))) {
		IrcLogger::getLogger()->log(IrcLogger::ERROR, "ChannelBuilder: Channel name contains only whitespaces or has non printable characters");
		return false;
	}
	return true;
}

// #include <iostream>
static bool isChannelPasswordValid(bool authorizeEmpty, std::string str) {
	if ((!authorizeEmpty && str.empty()) || str.length() > 100) {
		IrcLogger::getLogger()->log(IrcLogger::ERROR, "ChannelBuilder: Channel password is empty or too long");
		return false;
	}
	if ((!authorizeEmpty && StringUtils::isOnlyWhitespace(str)) || (!authorizeEmpty && !StringUtils::isPrintable(str))) {
		IrcLogger::getLogger()->log(IrcLogger::ERROR, "ChannelBuilder: Channel password contains only whitespaces or has non printable characters");
		return false;
	}
	return true;
}

void ChannelBuilder::clearBuilder() {
	this->name.clear();
	this->topic.clear();
	this->password.clear();
	this->properties.getInvited().clear();
	this->properties.getOperators().clear();
	this->properties.getBannedUsers().clear();
	this->properties.getMutedUsers().clear();
	this->properties.getUsersInChannel().clear();
}

Channel *ChannelBuilder::build() {

	//TODO: FIX NULL POINTER EXCEPTION WHEN PROPERTIES IS NOT SET
	//TODO: ADD GOOD EXCEPTIONS
	if (!isChannelNameValid(false, this->name))
		clearBuilder();
	if (!isChannelPasswordValid(true, this->password))
		clearBuilder();

	Channel* channel = new Channel();
	channel->setName(this->name);
	channel->setTopic(this->topic);
	channel->setPassword(this->password);
	//TODO: FIX NULL POINTER EXCEPTION WHEN PROPERTIES IS NOT SET
	//user->setProperties(this->properties);

	clearBuilder();

	return (channel);
}