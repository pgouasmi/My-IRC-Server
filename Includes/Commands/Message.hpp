//
// Created by pgouasmi on 4/18/24.
//

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "ICommand.hpp"


class Message : public ICommand {

public:
	Message();
	void execute(User *user, Channel *channel, std::vector<std::string>args);

};


#endif
