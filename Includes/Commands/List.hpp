#ifndef IRC_LIST_HPP
# define IRC_LIST_HPP

# include "ICommand.hpp"
# include "NumericReplies.hpp"

class List : public ICommand {
	public:
		List();
		void execute(User *user, Channel *channel, std::vector<std::string>args);
};

#endif //IRC_LIST_HPP
