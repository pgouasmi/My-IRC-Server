#include "List.hpp"

List::List()
{
	this->_name = "LIST";
	this->_description = "List all channels";
	this->_usage = "/list";
	this->_permissionLevel = 0;
}

//TODO: Ajouter les topic dans le /list
void List::execute(User *user, Channel *channel, std::vector<std::string> args)
{
	(void)args;
	(void)channel;
	ChannelCacheManager *manager = ChannelCacheManager::getInstance();
	std::list<Channel *> channelList = manager->getCache();
	for (std::list<Channel *>::iterator it = channelList.begin(); it != channelList.end(); it++)
		sendServerReply(user->getUserSocketFd(), RPL_LIST(user->getUserName(), "#" + (*it)->getName(), (*it)->getChannelsUsers().size(), (*it)->getTopic()), -1, DEFAULT);
	sendServerReply(user->getUserSocketFd(), RPL_LISTEND(user->getUserName()), -1, DEFAULT);
}