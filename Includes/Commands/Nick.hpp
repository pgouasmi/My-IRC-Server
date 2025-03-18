#ifndef IRC_NICK_HPP
# define IRC_NICK_HPP

# include "ICommand.hpp"

class Nick : public ICommand
{
	public :
		Nick();
		void execute(User *user, Channel *channel, std::vector<std::string>args);
};

#endif //IRC_NICK_HPP
