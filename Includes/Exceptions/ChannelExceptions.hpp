#ifndef CHANNELEXCEPTION_HPP
	#define CHANNELEXCEPTION_HPP
	#include <exception>
	#include <string>

	class ChannelBuildException : public std::exception
	{
		private:
			std::string message;
		public:
			ChannelBuildException(const std::string& str) throw();
			ChannelBuildException(const ChannelBuildException& other) throw();
			~ChannelBuildException() throw();
			const char *what() const throw();
	};

	class ChannelCacheException : public std::exception
	{
		private:
			size_t channelId;
			std::string message;
		public:
			ChannelCacheException(size_t channelId, const std::string& str) throw();
			ChannelCacheException(const ChannelCacheException& other) throw();
			~ChannelCacheException() throw();
			const char *what() const throw();
	};

	class ChannelGetException : public std::exception
	{
		private:
			std::string message;
		public:
			ChannelGetException(const std::string& str) throw();
			~ChannelGetException() throw();
			const char *what() const throw();
	};
#endif
