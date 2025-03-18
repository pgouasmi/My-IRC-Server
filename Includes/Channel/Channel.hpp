#ifndef CHANNEL_HPP
	#define CHANNEL_HPP

	#include <string>
	#include <vector>
	#include <algorithm>

	#include "ChannelProperties.hpp"
	#include <IRCPredicate.hpp>
	#include <PrimitivePredicate.hpp>
	#include "ChannelExceptions.hpp"
	#include "StringUtils.hpp"

	class User;

	class Channel {

		private:
			std::string name;
			size_t uniqueId;
			std::string topic;
			std::string password;
			ChannelProperties *properties;
			std::vector<User *> _usersInChannel;
			Channel();
		public:
			std::string getName() const;
			std::string getTopic() const;
			std::string getPassword() const;
			size_t getUniqueId() const;
			ChannelProperties *getProperties();
			void addUserToChannel(User *user);
			std::vector<User *> getChannelsUsers() const;
			User *getUserByNickname(const std::string &name) throw (ChannelGetException);
			bool isUserInChannel(const std::string &name);
			std::string getUserList();

			void removeUserFromChannel(User *user);
			void nameReplyAll();
			void nameReplyAllExceptCaller(const std::string &callerName);
			void joinReplyAll(const std::string &newUser);
			void whoReplyAll();
			void quitReplyAll(User *leftUser, const std::string &message);
			void topicReplyAll();
			void setTopic(const std::string& topic);
			void modeReplyAll();

			~Channel();
		private:
			friend class ChannelCacheManager;
			friend class ChannelBuilder;
			void setName(const std::string& name);
			void setPassword(const std::string& password);
			void setUniqueId(size_t uniqueId);
			void setProperties(ChannelProperties *properties);
	};

#endif