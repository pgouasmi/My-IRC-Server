#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP
	#include <string>
	#include <vector>
	#include "../User/User.hpp"
	#include "../Channel/Channel.hpp"
	#include "ArgumentsTypes.hpp"
	#include "StringUtils.hpp"
	#include "ChannelBuilder.hpp"
	#include "ChannelCacheManager.hpp"
	#include "UsersCacheManager.hpp"
	#include "NumericReplies.hpp"
	#include "IrcLogger.hpp"

	class ICommand {
		public:
			 std::string getName() const;
			 std::string getDescription() const;
			 std::string getUsage() const;
			 std::vector<ArgumentsType> getArgs() const;
			 int getPermission() const;
			 virtual void execute(User *user, Channel *channel, std::vector<std::string>args) = 0;
			 virtual ~ICommand();

	protected:
			//this vector will only contain MANDATORIES arguments for each command
			std::vector<ArgumentsType> _expectedArgs;
			std::string	_name;
			std::string _description;
			std::string _usage;
			int			_permissionLevel;
	};
#endif