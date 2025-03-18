#ifndef CHANNELBUILDER_HPP
	#define CHANNELBUILDER_HPP
	#include <string>
	#include "../Channel/Channel.hpp"
	#include "../Channel/ChannelProperties.hpp"

/**
 * @class ChannelBuilder
 * @brief A builder class for creating Channel objects.
 *
 * @author jbadaire
 *
 * This class provides a way to construct Channel objects in a step-by-step manner.
 * It includes setter methods for each property of the Channel class, and a build method
 * to finally create the Channel object.
 */
	class ChannelBuilder {
		private:
			std::string name;
			std::string topic;
			std::string password;
			ChannelProperties properties;

		public:
			/**
			 * @brief Sets the name of the Channel.
			 * @param name The name to set.
			 * @return A reference to the ChannelBuilder object.
			 * @author jbadaire
			 */
			ChannelBuilder& setName(const std::string& name);

			/**
			 * @brief Sets the topic of the Channel.
			 * @param topic The topic to set.
			 * @return A reference to the ChannelBuilder object.
			 * @author jbadaire
			 */
			ChannelBuilder& setTopic(const std::string& topic);

			/**
			 * @brief Sets the password of the Channel.
			 * @param password The password to set.
			 * @return A reference to the ChannelBuilder object.
			 * @author jbadaire
			 */
			ChannelBuilder& setPassword(const std::string& password);

			/**
			 * @brief Sets the properties of the Channel.
			 * @param properties The properties to set.
			 * @return A reference to the ChannelBuilder object.
			 * @author jbadaire
			 */
			ChannelBuilder& setProperties(const ChannelProperties& properties);

			/**
			 * @brief Clears the builder for reuse.
			 * @author jbadaire
			 */
			void clearBuilder();

			/**
			 * @brief Builds the Channel object.
			 * @return A reference to the built Channel object.
			 * @author jbadaire
			 */
			Channel *build();
	};

#endif
