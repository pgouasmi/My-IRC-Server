//
// Created by pgouasmi on 4/16/24.
//

#ifndef JOIN_HPP
#define JOIN_HPP

#include "ICommand.hpp"
#include <vector>


class Join : public ICommand {

public:
	Join();
	void execute(User *user, Channel *channel, std::vector<std::string>args);
};


#endif
