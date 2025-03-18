#ifndef IRC_PART_HPP
# define IRC_PART_HPP

# include "ICommand.hpp"

class Part : public ICommand
{
	public :
		Part();
		void execute(User *user, Channel *channel, std::vector<std::string>args);
		void sendPartMessageToChan(User *user, Channel *channel, const std::string& message);
};

#endif //IRC_PART_HPP
