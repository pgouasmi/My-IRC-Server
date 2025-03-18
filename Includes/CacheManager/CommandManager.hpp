//
// Created by pgouasmi on 4/16/24.
//

#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include <vector>
#include <map>
#include "ICommand.hpp"
#include "Join.hpp"
#include "Message.hpp"


class CommandManager {

private:
	CommandManager();

	static CommandManager *instance;
	std::map<std::string, ICommand *> _commandsMap;

public:
	~CommandManager();
	static CommandManager *getInstance();
	ICommand *getCommand(const std::string &key) const;
	bool doesCommandExist(const std::string &key) const;
};


#endif //INC_42_IRC_COMMANDMANAGER_HPP
