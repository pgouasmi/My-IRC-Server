//
// Created by pgouasmi on 4/16/24.
//

#include "ICommand.hpp"

std::string ICommand::getName() const
{
	return this->_name;
}

std::string ICommand::getDescription() const
{
	return this->_description;
}

std::string ICommand::getUsage() const
{
	return this->_usage;
}

std::vector<ArgumentsType> ICommand::getArgs() const
{
	return this->_expectedArgs;
}

int ICommand::getPermission() const
{
	return this->_permissionLevel;
}

ICommand::~ICommand() {}


