//
// Created by pgouasmi on 4/23/24.
//

#include "Mode.hpp"

Mode::Mode()
{
	this->_name = "MODE";
	this->_permissionLevel = 1;
	this->_usage = "/mode #canal +b user // /mode your_nickname +a";
	this->_description = "Modifies attributes or permissions for a user or a channel";

	(void)this->_expectedArgs;
}

static void handleBanMode(User *user, std::string channelName, std::vector<std::string> args, int mode)
{
	(void)user;

	Channel *targetChannel = ChannelCacheManager::getInstance()->getFromCacheString(channelName);
	ChannelProperties *properties = targetChannel->getProperties();
	User *targetUser = UsersCacheManager::getInstance()->getFromNickname(args[1]);

	if (mode == PLUS)
	{
		if (targetChannel->isUserInChannel(targetUser->getNickname()))
		{
			std::string message = "You've been banned from #" + channelName;
			sendServerReply(targetUser->getUserSocketFd(), RPL_NOTICE(targetUser->getNickname(), targetUser->getUserName(), user->getNickname(), message), -1, DEFAULT);
			sendServerReply(user->getUserSocketFd(), RPL_ADDMODEUSER(user->getNickname(), targetUser->getNickname(), "+b", channelName), -1, DEFAULT);
			properties->addUserToBannedUsers(targetUser->getUniqueId());
		}
		else
			sendServerReply(user->getUserSocketFd(), ERR_USERNOTINCHANNEL(user->getNickname(), targetUser->getNickname(), channelName), -1, DEFAULT);
	}
	else
	{
		if (properties->isUserBanned(targetUser->getUniqueId())) {
			std::string message = "You've been unbanned from #" + channelName;
			sendServerReply(targetUser->getUserSocketFd(),
							RPL_NOTICE(targetUser->getNickname(), targetUser->getUserName(), user->getNickname(),
									   message), -1, DEFAULT);
			properties->removeUserFromBannedUsers(targetUser->getUniqueId());
		}
		else
			sendServerReply(user->getUserSocketFd(), ERR_INVALIDMODEGENERAL(user->getNickname(), channelName, "b", "this user is not banned"), -1, DEFAULT);
	}

}

static void handleKeyMode(User *user, std::string channelName, std::vector<std::string> args, int mode)
{
	Channel *targetChannel = ChannelCacheManager::getInstance()->getFromCacheString(channelName);
	ChannelProperties *properties = targetChannel->getProperties();
	std::string modeRpl;

	if (mode == PLUS)
	{
		modeRpl += '+';
		if (args.size() < 2)
		{
			sendServerReply(user->getUserSocketFd(), ERR_INVALIDMODEGENERAL(user->getNickname(), channelName, "+k", "no password provided"), -1, DEFAULT);
			return ;
		}
		std::string keyword = args[1];
		if (!StringUtils::isAlpha(keyword))
		{
			sendServerReply(user->getUserSocketFd(), ERR_INVALIDMODEPARAM(user->getNickname(), channelName, "+K", keyword), -1, DEFAULT);
			return;
		}
		properties->setPassword(keyword);
		properties->setPasswordStatus(true);
		properties->addModeToChannel(user->getUniqueId(), 'k');
	}
	else if (mode == MINUS)
	{
		modeRpl += '-';
		properties->setPassword("");
		properties->setPasswordStatus(false);
		properties->removeModeToChannel(user->getUniqueId(), 'k');
	}
	modeRpl += 'k';
	sendServerReply(user->getUserSocketFd(), RPL_ADDMODE(user->getNickname(), user->getNickname(), modeRpl, channelName), -1, DEFAULT);
}

static void handleLimitMode(User *user, std::string channelName, std::vector<std::string> args, int mode)
{
	Channel *targetChannel = ChannelCacheManager::getInstance()->getFromCacheString(channelName);
	ChannelProperties *properties = targetChannel->getProperties();
	std::string modeRpl;

	if (mode == PLUS)
	{
		modeRpl += "+";
		modeRpl += 'l';
		if (args.size() < 2)
		{
			sendServerReply(user->getUserSocketFd(), ERR_ADDMODE(user->getNickname(), channelName, modeRpl, "Not enough parameters"), -1, DEFAULT);
			return ;
		}
		if (!StringUtils::isOnlyDigits(args[1]))
		{
			sendServerReply(user->getUserSocketFd(), ERR_ADDMODE(user->getNickname(), channelName, modeRpl, "Argument must be digits"), -1, DEFAULT);
			return ;
		}

		char *pEnd;
		double newLimit = strtod(args[1].c_str(), &pEnd);
		if (newLimit > 100)
		{
			sendServerReply(user->getUserSocketFd(), ERR_ADDMODE(user->getNickname(), channelName, modeRpl, "Limit is too high"), -1, DEFAULT);
			return ;
		}
		try {
			properties->addModeToChannel(user->getUniqueId(), 'l');
			properties->setUserLimit(static_cast<int>(newLimit));
			properties->setUserLimitStatus(true);
		}
		catch (std::exception &e) {
			IrcLogger *logger = IrcLogger::getLogger();
			logger->log(IrcLogger::INFO, e.what());
			properties->setUserLimit(static_cast<int>(newLimit));
			properties->setUserLimitStatus(true);
//			sendServerReply(user->getUserSocketFd(), ERR_ADDMODE(user->getNickname(), channelName, modeRpl, "Channel already has this mode"), -1, DEFAULT);
			return ;
		}
	}
	else if (mode == MINUS)
	{
		modeRpl += "-";
		modeRpl += "l";
		try {
			properties->removeModeToChannel(user->getUniqueId(), 'l');
			properties->setUserLimit(-1);
			properties->setUserLimitStatus(false);
			properties->removeModeToChannel(user->getUniqueId(), 'l');
		}
		catch (std::exception &e) {
			IrcLogger *logger = IrcLogger::getLogger();
			logger->log(IrcLogger::ERROR, e.what());
			sendServerReply(user->getUserSocketFd(), ERR_ADDMODE(user->getNickname(), channelName, modeRpl, "Channel does not have this mode"), -1, DEFAULT);
			return ;
		}
	}
	sendServerReply(user->getUserSocketFd(), RPL_ADDMODE(user->getNickname(), user->getNickname(), modeRpl, channelName), -1, DEFAULT);
}

static void handleUserMode(User *user, std::string channelName, std::vector<std::string> args, int mode, char c)
{
	Channel *targetChannel = ChannelCacheManager::getInstance()->getFromCacheString(channelName);
	ChannelProperties *properties = targetChannel->getProperties();
	User *targetUser = UsersCacheManager::getInstance()->getFromNickname(args[1]);

	std::string repMode = ":MODE ";
	if (mode == PLUS)
		repMode += '+';
	else
		repMode += '-';
	repMode += c;

	if (args.size() < 2){
		sendServerReply(user->getUserSocketFd(), ERR_ADDMODE(user->getNickname(), channelName, repMode, "Not enough parameters"), -1, DEFAULT);
		return ;
	}
	try {
		if (mode == PLUS)
		{
			properties->addModeToUser(targetUser->getUniqueId(), user->getUniqueId(), c);
			sendServerReply(user->getUserSocketFd(), RPL_ADDMODEUSER(user->getNickname(), targetUser->getNickname(), repMode, channelName), -1, DEFAULT);
		}
		else if (mode == MINUS) {
			properties->removeModeToUser(targetUser->getUniqueId(), user->getUniqueId(), c);
			sendServerReply(user->getUserSocketFd(), RPL_ADDMODEUSER(user->getNickname(), targetUser->getNickname(), repMode, channelName), -1, DEFAULT);
		}
		if (c == 'o') {
			if (mode == PLUS)
				sendServerReply(targetUser->getUserSocketFd(), RPL_YOUREOPER(targetUser->getNickname(), channelName), -1, DEFAULT);
			targetChannel->nameReplyAll();
		}
		sendServerReply(targetUser->getUserSocketFd(), RPL_ADDMODEUSER(targetUser->getNickname(), targetUser->getNickname(), repMode, channelName), -1, DEFAULT);
	}
	catch (std::exception &e) {
		IrcLogger *logger = IrcLogger::getLogger();
		logger->log(IrcLogger::ERROR, e.what());

		repMode += ": ";
		std::string str = e.what();
		if (str == "Error: User doesn't have this mode")
		{
			repMode += targetUser->getNickname();
			repMode += " does not have this mode";
		}
		else
		{
			repMode += targetUser->getNickname();
			repMode += " already has this mode";
		}
		sendServerReply(user->getUserSocketFd(), ERR_INVALIDMODEGENERAL(user->getNickname(), channelName, repMode, "impossible mode modification"), -1, DEFAULT);
	}
}

static void handleChannelMode(User *user, std::string channelName, std::vector<std::string> args, int mode, char c)
{
	Channel *targetChannel = ChannelCacheManager::getInstance()->getFromCacheString(channelName);
	ChannelProperties *properties = targetChannel->getProperties();
	std::string modeRpl;

	if (mode == PLUS)
		modeRpl += '+';
	if (mode == MINUS)
		modeRpl += '-';
	modeRpl += c;

	if (args.size() < 1){
		sendServerReply(user->getUserSocketFd(), ERR_ADDMODE(user->getNickname(), channelName, modeRpl, "Not enough parameters"), -1, DEFAULT);
		return ;
	}

	if (mode == PLUS) {
		try {
			properties->addModeToChannel(user->getUniqueId(), c);
		}
		catch (std::exception &e) {
			IrcLogger *logger = IrcLogger::getLogger();
			logger->log(IrcLogger::ERROR, e.what());

			sendServerReply(user->getUserSocketFd(), ERR_ADDMODE(user->getNickname(), channelName, modeRpl, "Channel already has this mode"), -1, DEFAULT);
			return;
		}
	}
	else if (mode == MINUS) {
		try {
			properties->removeModeToChannel(user->getUniqueId(), c);
		}
		catch (std::exception &e) {
			IrcLogger *logger = IrcLogger::getLogger();
			logger->log(IrcLogger::ERROR, e.what());

			sendServerReply(user->getUserSocketFd(), ERR_ADDMODE(user->getNickname(), channelName, modeRpl, "Channel does not have this mode"), -1, DEFAULT);
			return;
		}
	}
	sendServerReply(user->getUserSocketFd(), RPL_ADDMODE(user->getNickname(), user->getNickname(), modeRpl, channelName), -1, DEFAULT);
}

static bool isAValidModeChar(char c)
{
	std::string validChars = "ovqabklmt+-";
	if (validChars.find(c) != std::string::npos)
		return true;
	return false;
}

void Mode::execute(User *user, Channel *channel, std::vector<std::string> args)
{
	

	(void) channel;
	(void) user;

	std::string channelNew;
	Channel *targetChannel;

	if (args[0][0] != '#') {
		sendServerReply(user->getUserSocketFd(), ERR_NEEDMOREPARAMS(user->getNickname(), this->_name), -1, DEFAULT);
		return;
	}

	channelNew = args[0];
	StringUtils::trim(channelNew, "#");
	args.erase(args.begin());

	try {
		targetChannel = ChannelCacheManager::getInstance()->getFromCacheString(channelNew);
	}
	catch (const std::exception &e) {
	    IrcLogger::getLogger()->log(IrcLogger::ERROR, e.what());
		sendServerReply(user->getUserSocketFd(), ERR_NOSUCHCHANNEL(user->getNickname(), channelNew), -1, DEFAULT);
		return;
	}
	ChannelProperties *properties = targetChannel->getProperties();


	if (properties->isUserOnChannel(user->getUniqueId()) == false)
	{
		sendServerReply(user->getUserSocketFd(), ERR_NOTONCHANNEL(user->getNickname(), channelNew), -1, DEFAULT);
		return ;
	}

	if (args.size() == 0)
	{
		if (properties->isPasswordSet() == true)
			sendServerReply(user->getUserSocketFd(), RPL_CHANNELMODEISWITHKEY(user->getNickname(), channelNew, properties->getChannelModes(), properties->getPassword()), -1, DEFAULT);
		else
			sendServerReply(user->getUserSocketFd(), RPL_CHANNELMODEIS(user->getNickname(), channelNew, properties->getChannelModes()), -1, DEFAULT);
//		sendServerReply(user->getUserSocketFd(), MODE_CHANNELMSG(channelNew, properties->getChannelModes()), -1, DEFAULT);
		return ;
	}

	if (properties->isUserOperator(user->getUniqueId()) == false)
	{
		sendServerReply(user->getUserSocketFd(), ERR_CHANOPRIVSNEEDED(user->getNickname(), channelNew), -1, DEFAULT);
		return ;
	}

	bool New = true;

	std::vector<std::vector<std::string > >argV;
	for (std::vector<std::string>::iterator it = args.begin() ; it != args.end() ; ++it)
	{
		std::string res;
		while (it != args.end()) {
			if (New)
			{
				res += *it;
				res += ' ';
				it++;
				New = false;
			}
			else
			{
				if (((*it)[0] != '+' && (*it)[0] != '-'))
				{
					New = false;
					res += *it;
					res += ' ';
					it++;
					if (it == args.end())
						break;
				}
				else
				{
					New = true;
					break;
				}
			}
		}
		StringUtils::trim(res, " ");
		argV.push_back(StringUtils::split(res, ' '));
		if (it == args.end())
			break ;
		res.clear();
		New = true;
		if ((*it)[0] == '+' || (*it)[0] == '-')
			--it;
	}

	std::string userModes = "ovq";
	std::string channelModes = "ti";

	bool hasModification = false;
	for (std::vector<std::vector<std::string> >::iterator it = argV.begin() ; it != argV.end() ; ++it)
	{
		int mode = 42;
		for (std::string::iterator sIt = (*it)[0].begin() ; sIt != (*it)[0].end() ; ++sIt)
		{
			if (isAValidModeChar(*sIt) == false)
			{
				sendServerReply(user->getUserSocketFd(), ERR_INVALIDMODEGENERAL(user->getNickname(), channelNew, *sIt, "Invalid mode"), RED, BOLDR);
				continue ;
			}
			if (*sIt == '+')
				mode = PLUS;
			if (*sIt == '-')
				mode = MINUS;
			if (*sIt == 'k')
			{
				try {
					handleKeyMode(user, channelNew, *it, mode);
					hasModification = true;
				}
				catch (std::exception &e) {
					sendServerReply(user->getUserSocketFd(), ERR_INVALIDMODEGENERAL(user->getNickname(), channelNew, (*it)[0], "Wrong usage"), -1, DEFAULT);
				}
			}
			if (mode == 42)
				sendServerReply(user->getUserSocketFd(), ERR_NEEDMOREPARAMS(user->getNickname(), "MODE"), -1, DEFAULT);
			else if (*sIt == 'l')
			{
				try {
					handleLimitMode(user, channelNew, *it, mode);
					hasModification = true;
				}
				catch (std::exception &e) {
				    IrcLogger::getLogger()->log(IrcLogger::ERROR, e.what());
				}
			}
			else if (*sIt == 'b')
			{
				try {
					handleBanMode(user, channelNew, *it, mode);
				}
				catch (const UserCacheExceptionString &e) {
					sendServerReply(user->getUserSocketFd(), ERR_INVALIDMODEGENERAL(user->getNickname(), channelNew, (*it)[0], "No such user"), -1, DEFAULT);
				}
				catch (const std::exception &e) {
					IrcLogger::getLogger()->log(IrcLogger::ERROR, e.what());
				}
			}
			else if (userModes.find(*sIt) != std::string::npos) {
				try {
					handleUserMode(user, channelNew, *it, mode, *sIt);
				}
				catch (UserCacheExceptionString &e) {
					sendServerReply(user->getUserSocketFd(), ERR_USERNOTINCHANNEL(user->getNickname(), args[1], channelNew), -1, DEFAULT);
				    IrcLogger::getLogger()->log(IrcLogger::ERROR, e.what());
				}
			}
			else if (channelModes.find(*sIt) != std::string::npos) {
				try {
					handleChannelMode(user, channelNew, *it, mode, *sIt);
					hasModification = true;
				}
				catch (std::exception &e) {
				    IrcLogger::getLogger()->log(IrcLogger::ERROR, e.what());
				}
			}
		}
	}
	if (hasModification)
		targetChannel->modeReplyAll();
}