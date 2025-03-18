#ifndef CHANNELMANAGER_HPP
#define CHANNELMANAGER_HPP

	#include "Channel.hpp"
	#include "ChannelExceptions.hpp"
	#include <list>

	class 	ChannelCacheManager
	{
		private:
			static ChannelCacheManager* instance;
			std::list<Channel *> channels;
			size_t uniqueIdCounter;
			ChannelCacheManager();

		public:
			void addToCache(Channel *channel) throw(ChannelCacheException);
			Channel *getFromCache(size_t uniqueId) throw(ChannelCacheException);
			Channel *getFromCacheString(const std::string &name) throw(ChannelCacheException);
			bool doesChannelExist(const std::string &name);
			void deleteFromCache(size_t uniqueId) throw(ChannelCacheException);
			std::list<Channel *> getCache();
			static ChannelCacheManager* getInstance();
			~ChannelCacheManager();
	};


#endif
