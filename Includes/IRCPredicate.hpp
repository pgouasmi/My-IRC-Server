#ifndef COMPARATOR_HPP
	#define COMPARATOR_HPP

	#include "User.hpp"
	#include "Channel.hpp"

	#include <vector>

	class ChannelPredicate
	{
		private:
			size_t channelId;
		public:
			explicit ChannelPredicate(size_t channelId);
			bool operator () (Channel *channel);
			bool operator == (size_t channelID);
	};

	class ChannelPredicateName
	{
		private:
			std::string channelName;
		public:
			explicit ChannelPredicateName(const std::string &channelName);
			bool operator () (Channel *channel);
			bool operator == (const std::string &channelName);
	};

	class UserPredicate
	{
			private:
				size_t userId;
			public:
				explicit UserPredicate(size_t userId);
				bool operator () (User *user);
				bool operator == (size_t userId);
	};

	class UserPredicateFD
	{
			private:
				int socketFD;
			public:
				explicit UserPredicateFD(int fd);
				bool operator () (User *user);
				bool operator == (int fd);
	};


	class UserPredicateNickname
	{
		private:
			std::string nickname;
		public:
			explicit UserPredicateNickname(const std::string &value);
			bool operator () (User *user);
			bool operator == (const std::string &value);
	};

	class UserPredicateUsername
	{
		private:
			std::string username;
		public:
			explicit UserPredicateUsername(const std::string &value);
			bool operator () (User *user);
			bool operator == (const std::string &value);
	};
#endif
