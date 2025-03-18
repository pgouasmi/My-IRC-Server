
#ifndef INC_42_IRC_WHOIS_HPP
# define INC_42_IRC_WHOIS_HPP

#include "ICommand.hpp"

class WhoIs : public ICommand
{
	public:
		WhoIs();
		void execute(User *user, Channel *channel, std::vector<std::string> args);
};

#endif //INC_42_IRC_WHOIS_HPP
