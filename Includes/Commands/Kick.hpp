//
// Created by pgouasmi on 4/29/24.
//

#include "ICommand.hpp"

#ifndef KICK_HPP
#define KICK_HPP


class Kick : public ICommand {
public:

	Kick();
	void execute(User *user, Channel *channel, std::vector<std::string>args);

};


#endif
