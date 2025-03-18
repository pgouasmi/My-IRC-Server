//
// Created by pgoua on 21/04/2024.
//

#ifndef INVITE_HPP
#define INVITE_HPP

#include "ICommand.hpp"


class Invite : public ICommand {
public:
	Invite();
	void execute(User *user, Channel *channel, std::vector<std::string>args);
};


#endif //INC_42_IRC_INVITE_HPP
