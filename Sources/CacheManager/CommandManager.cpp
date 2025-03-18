//
// Created by pgouasmi on 4/16/24.
//

#include "CommandManager.hpp"

#include <Nick.hpp>
#include <Part.hpp>
#include <Ping.hpp>
#include <Pong.hpp>
#include "Join.hpp"
#include "Part.hpp"
#include "Ping.hpp"
#include "Quit.hpp"
#include "Who.hpp"
#include "WhoIs.hpp"
#include "Invite.hpp"
#include "List.hpp"
#include "Mode.hpp"
#include "Topic.hpp"
#include "Kick.hpp"

CommandManager* CommandManager::instance = NULL;

CommandManager::CommandManager()
{
	//NEED TO ADD
	//creer kick
	this->_commandsMap["JOIN"] = new Join();
	this->_commandsMap["PART"] = new Part();
	this->_commandsMap["QUIT"] = new Quit();
	this->_commandsMap["PRIVMSG"] = new Message();
	this->_commandsMap["MSG"] = new Message();
	this->_commandsMap["WHO"] = new Who();
	this->_commandsMap["who"] = new Who();
	this->_commandsMap["WHOIS"] = new WhoIs();
	this->_commandsMap["whois"] = new WhoIs();
	this->_commandsMap["INVITE"] = new Invite();
	this->_commandsMap["LIST"] = new List();
	this->_commandsMap["PING"] = new Ping();
	this->_commandsMap["PONG"] = new Pong();
	this->_commandsMap["MODE"] = new Mode();
	this->_commandsMap["NICK"] = new Nick();
	this->_commandsMap["TOPIC"] = new Topic();
	this->_commandsMap["KICK"] = new Kick();
}

CommandManager::~CommandManager()
{
	for (std::map<std::string, ICommand *>::iterator it = this->_commandsMap.begin(); it != this->_commandsMap.end(); ++it) {
		delete it->second;
	}
}

CommandManager *CommandManager::getInstance()
{
	return instance == NULL ? instance = new CommandManager() : instance;
}

ICommand *CommandManager::getCommand(const std::string &key) const
{
	if (this->doesCommandExist(key))
		return this->_commandsMap.at(key);
	return NULL;
}

bool CommandManager::doesCommandExist(const std::string &key) const
{
	return this->_commandsMap.find(key) != _commandsMap.end() ? true : false;
}

