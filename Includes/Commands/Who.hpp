//
// Created by pgoua on 21/04/2024.
//

#ifndef WHO_HPP
#define WHO_HPP

#include "ICommand.hpp"


class Who : public ICommand {

public:
	Who();
	void execute(User *user, Channel *channel, std::vector<std::string>args);
};


#endif
