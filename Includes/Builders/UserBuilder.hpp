#ifndef USERBUILDER_HPP
	#define USERBUILDER_HPP
	#include <string>
	#include <vector>

	#include "UserExceptions.hpp"
	#include "User.hpp"
	#include "UserProperties.hpp"
	#include "IrcLogger.hpp"
	#include "StringUtils.hpp"
	#include "UsersCacheManager.hpp"

	#include "Configuration.hpp"

	#include "NumericReplies.hpp"

/**
 * @class UserBuilder
 * @brief A builder class for creating User objects.
 * @author jbadaire
 *
 * This class provides a way to construct User objects in a step-by-step manner.
 * It includes setter methods for each property of the User class, and a build method
 * to finally create the User object.
 */
	class UserBuilder
	{
		private:
			std::string userName;
			std::string realName;
			std::string nickname;
			std::string ipAddr;
			std::string password;
			int userSocketFd;
			size_t uniqueId;
			size_t builderTimeout;
			UserProperties properties;
			std::vector<std::string> connectionInfos;

		public:
			UserBuilder();

			/**
			 * @brief Sets the name of the User.
			 * @param name The name to set.
			 * @return A reference to the UserBuilder object.
			 * @author jbadaire
			 */
			UserBuilder &setName(const std::string &name);

			/**
			 * @brief Sets the real name of the User.
			 * @param realName The real name to set.
			 * @return A reference to the UserBuilder object.
			 * @author jbadaire
			 */
			UserBuilder &setRealName(const std::string &realName);

			/**
			 * @brief Sets the nickname of the User.
			 * @param nickname The nickname to set.
			 * @return A reference to the UserBuilder object.
			 * @author jbadaire
			 */
			UserBuilder &setNickname(const std::string &nickname);

			/**
			 * @brief Sets the IP address of the User.
			 * @param ipAddr The IP address to set.
			 * @return A reference to the UserBuilder object.
			 * @author jbadaire
			 */
			UserBuilder &setIpAddr(const std::string &ipAddr);

			/**
			 * @brief Sets the socket ID of the User.
			 * @param userSocketFd The socket ID to set.
			 * @return A reference to the UserBuilder object.
			 * @author jbadaire
			 */
			UserBuilder &setUserSocketId(int userSocketFd);

			/**
			 * @brief Sets the timestamp timeout of the User.
			 * @param timeout The timeout to set.
			 * @return A reference to the UserBuilder object.
			 * @author jbadaire
			 */
			UserBuilder &setBuilderTimeout(size_t timeout);

			/**
			 * @brief Sets the properties of the User.
			 * @param properties The properties to set.
			 * @return A reference to the UserBuilder object.
			 * @author jbadaire
			 */
			UserBuilder &setProperties(const UserProperties &properties);

			size_t getTimeout() const;

			/**
			 * @brief Clears the builder for reuse.
			 * @author jbadaire
			 */
			void clearBuilder();

			/**
			 * @brief Builds the User object.
			 * @return A reference to the built User object.
			 * @throws UserBuildException if essentials properties for IRC are invalid.
			 * @author jbadaire
			 */
			User *build();

			UserBuilder &fillBuffer(const std::string data, int incomingFD);

			bool 	isBuilderComplete() throw (UserBuildException);
	};

//	std::ostream  &operator<<(std::ostream &os, const UserBuilder &obj);

	
#endif
