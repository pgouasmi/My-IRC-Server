//
// Created by pgouasmi on 4/25/24.
//

#ifndef INC_42_IRC_TOPIC_HPP
#define INC_42_IRC_TOPIC_HPP

#include "ICommand.hpp"


class Topic : public ICommand {
public:
	Topic();
	void execute(User *user, Channel *channel, std::vector<std::string>args);
};


#endif //INC_42_IRC_TOPIC_HPP
