#ifndef USER_HPP
	#define USER_HPP

	#include <string>
	#include <vector>

enum BUFFERSTATUS {
	OK,
	WAITING,
	KO
};
//	#include "Channel.hpp"
	class Channel;

	class UserProperties;

	class User {
		private:
			User();
			std::string userName;
			std::string realName;
			std::string nickname;
			std::string ipAddr;

			std::string receivedBuffer;

			int userSocketFd;
			size_t lastPingTimestamp;
			size_t uniqueId;
			UserProperties* properties;
			Channel *currentChannel;
			std::vector<Channel *> channelList;

		public:
			std::string getUserName() const;
			std::string getRealName() const;
			std::string getNickname() const;
			std::string getIpAddr() const;
			int getUserSocketFd() const;
			size_t getLastPingTimestamp() const;
			size_t getUniqueId() const;
			UserProperties getProperties() const;
			Channel *getCurrentChannel() const;
			std::vector<Channel *> getChannelList() const;
			void addChannelToList(Channel *newChannel);
			void setNickname(const std::string& nickname);
			void setLastPingTimestamp(size_t timestamp);

			void addToBuffer(const std::string &incomingBuffer);
			std::string &getReceivedBuffer();
			int isBufferValid() const;
			void clearBuffer();

		private:
			friend class UserBuilder;
			friend class UsersCacheManager;
			void setName(const std::string& name);
			void setRealName(const std::string& realName);
			void setIpAddr(const std::string& ipAddr);
			void setUserSocketId(int userSocketFd);
			void setUniqueId(const size_t& uniqueId);
			void setProperties(const UserProperties& properties);
	};

#endif
