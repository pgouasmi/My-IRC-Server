#include "Channel.hpp"
#include "ChannelProperties.hpp"
#include "IrcLogger.hpp"

#include "User.hpp"

Channel::Channel()
{
	this->properties = new ChannelProperties();
}

Channel::~Channel()
{
	delete this->properties;
}

std::string Channel::getName() const
{
	return this->name;
}

std::string Channel::getTopic() const
{
	return this->topic;
}

std::string Channel::getPassword() const
{
	return this->password;
}

size_t Channel::getUniqueId() const
{
	return this->uniqueId;
}

ChannelProperties *Channel::getProperties()
{
	return this->properties;
}

void Channel::setName(const std::string &name)
{
	this->name = name;
}

void Channel::setTopic(const std::string &topic)
{
	this->topic = topic;
}

void Channel::setPassword(const std::string &password)
{
	this->password = password;
}

void Channel::setUniqueId(size_t uniqueId)
{
	this->uniqueId = uniqueId;
}

void Channel::setProperties(ChannelProperties *properties)
{
	this->properties = properties;
}

void Channel::addUserToChannel(User *user)
{
	this->_usersInChannel.push_back(user);
}

std::vector<User *> Channel::getChannelsUsers() const
{
	return this->_usersInChannel;
}

User *Channel::getUserByNickname(const std::string &name)  throw (ChannelGetException)
{
	std::vector<User *>::iterator it = std::find_if(this->_usersInChannel.begin(), this->_usersInChannel.end(), UserPredicateNickname(name));
	if (it != this->_usersInChannel.end())
		return *it;
	std::string chanName = this->name;
	throw ChannelGetException(("No user called " + name + "in channel #" + chanName));
}

bool Channel::isUserInChannel(const std::string &name)
{
	std::vector<User *>::iterator it = std::find_if(this->_usersInChannel.begin(), this->_usersInChannel.end(), UserPredicateNickname(name));
	if (it != this->_usersInChannel.end())
		return true;
	return false;
}

std::string Channel::getUserList()
{
	std::string result;

	ChannelProperties *properties = this->getProperties();
	for (std::vector<User *>::iterator it = this->_usersInChannel.begin() ; it != this->_usersInChannel.end() ; ++it)
	{
		if (properties->doesUserHaveMode((*it)->getUniqueId(), 'o'))
			result += "@";
		result += (*it)->getNickname();
		result += " ";
	}
	StringUtils::trim(result, " ");
	return result;
}

void Channel::removeUserFromChannel(User *user) {
	std::vector<User *>::iterator it = find(this->_usersInChannel.begin(), this->_usersInChannel.end(), user);
	if (it != this->_usersInChannel.end()) {
		this->_usersInChannel.erase(it);
		this->properties->getMap().erase(user->getUniqueId());
	}
}

void Channel::whoReplyAll()
{
	ChannelProperties *properties = this->getProperties();
	std::map<size_t, std::string> map = properties->getMap();

	for (std::map<size_t , std::string >::iterator it =map.begin() ; it != map.end() ; ++it)
	{
		User *currentUser = UsersCacheManager::getInstance()->getFromCache(it->first);
		sendServerReply(currentUser->getUserSocketFd(), RPL_NAMREPLY(currentUser->getNickname(), "<@|*=|:|>", this->name, this->getUserList()), -1, DEFAULT);
	}
}

void Channel::nameReplyAll()
{
	ChannelProperties *properties = this->getProperties();
	std::map<size_t, std::string> map = properties->getMap();

	for (std::map<size_t, std::string>::iterator it = map.begin(); it != map.end(); ++it) {
		try {
			User *currentUser = UsersCacheManager::getInstance()->getFromCache(it->first);
			sendServerReply(currentUser->getUserSocketFd(),
							RPL_NAMREPLY(currentUser->getNickname(), "<@|*=|:|>", this->name, this->getUserList()), -1,
							DEFAULT);
			sendServerReply(currentUser->getUserSocketFd(), RPL_ENDOFNAMES(currentUser->getNickname(), this->name), -1, DEFAULT);
		}
		catch (const UserCacheException &ignored) {}
	}
}

void Channel::nameReplyAllExceptCaller(const std::string &callerName)
{
    if (this->properties == NULL) {
        IrcLogger::getLogger()->log(IrcLogger::ERROR, "Channel does not exist anymore   ");
        return;
    }
	ChannelProperties *properties = this->getProperties();
	std::map<size_t, std::string> map = properties->getMap();

	for (std::map<size_t , std::string >::iterator it =map.begin() ; it != map.end() ; ++it)
	{
		User *currentUser = UsersCacheManager::getInstance()->getFromCache(it->first);
		if (currentUser->getNickname() != callerName) {
			sendServerReply(currentUser->getUserSocketFd(),
							RPL_NAMREPLY(currentUser->getNickname(), "<@|*=|:|>", this->name, this->getUserList()), -1,
							DEFAULT);
			sendServerReply(currentUser->getUserSocketFd(), RPL_ENDOFNAMES(currentUser->getNickname(), this->name), -1, DEFAULT);
		}

	}
}

void Channel::joinReplyAll(const std::string &newUser)
{
	ChannelProperties *properties = this->getProperties();
	std::map<size_t, std::string> map = properties->getMap();

	for (std::map<size_t , std::string >::iterator it =map.begin() ; it != map.end() ; ++it)
	{
		User *currentUser = UsersCacheManager::getInstance()->getFromCache(it->first);
		if (currentUser->getNickname() != newUser)
			sendServerReply(currentUser->getUserSocketFd(), RPL_JOIN(user_id(currentUser->getNickname(), newUser), this->getName()), -1, DEFAULT);
	}
}

void Channel::quitReplyAll(User *leftUser, const std::string &message)
{
	std::vector<User *> userList = this->getChannelsUsers();
	if (userList.empty()) {
			delete this;
	}
	for (std::vector<User *>::iterator it = userList.begin(); it != userList.end(); it++) {
		sendServerReply((*it)->getUserSocketFd(),
						RPL_QUIT(user_id(leftUser->getNickname(), leftUser->getUserName()), message),
						-1, DEFAULT);
	}
}

void Channel::topicReplyAll()
{
	std::vector<User *>::iterator it = this->_usersInChannel.begin();
	while (it != this->_usersInChannel.end()) {
		sendServerReply((*it)->getUserSocketFd(), RPL_TOPIC((*it)->getNickname(), this->name, this->getProperties()->getTopic()),
		GREEN, BOLDR);
		++it;
	}
}

void Channel::modeReplyAll()
{
	// std::cout << "modeReplyAll" << std::endl;
	std::vector<User *> userList = this->_usersInChannel;
	// std::cout << "list size: " << userList.size() << std::endl;
	std::string channelModes = this->properties->getChannelModes();

	std::vector<User *>::iterator it = userList.begin();

	while (it != userList.end()) {
		sendServerReply((*it)->getUserSocketFd(), RPL_CHANNELMODEIS((*it)->getNickname(), this->name, channelModes), -1, DEFAULT);
		++it;
	}
}