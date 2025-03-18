#ifndef SERVEREXCEPTIONS_HPP
	#define SERVEREXCEPTIONS_HPP
	#include <exception>
	#include <string>
	class ServerStartingException : public std::exception
	{
		private:
			std::string message;
		public:
			ServerStartingException(const std::string& message) throw();
			ServerStartingException(const ServerStartingException& other) throw();
			~ServerStartingException() throw();
			const char *what() const throw();
	};

	class ServerInitializationException : public std::exception
	{
		private:
			std::string message;
		public:
			ServerInitializationException(const std::string& message) throw();
			ServerInitializationException(const ServerInitializationException& other) throw();
			~ServerInitializationException() throw();
			const char *what() const throw();
	};

#endif
