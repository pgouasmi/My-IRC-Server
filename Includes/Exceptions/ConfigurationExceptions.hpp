#ifndef CONFIGURATIONEXCEPTIONS_HPP
	#define CONFIGURATIONEXCEPTIONS_HPP
	#include <exception>
	#include <string>
	class ConfigurationIOException : public std::exception
	{
		private:
			std::string message;
		public:
			ConfigurationIOException(const std::string& message) throw();
			ConfigurationIOException(const ConfigurationIOException& other) throw();
			~ConfigurationIOException() throw();
			const char *what() const throw();
	};
#endif
