#ifndef USEREXCEPTIONS_HPP
#define USEREXCEPTIONS_HPP
#include <exception>
#include <string>

class UserConnectionException : public std::exception
{
	private:
		std::string message;
	public:
		UserConnectionException(const std::string& str) throw();
		UserConnectionException(const UserConnectionException& other) throw();
		~UserConnectionException() throw();
		const char *what() const throw();
};

class UserBuildException : public std::exception
{
	private:
		std::string message;
	public:
		UserBuildException(const std::string& str) throw();
		UserBuildException(const UserBuildException& other) throw();
		~UserBuildException() throw();
		const char *what() const throw();
};

class UserCacheException : public std::exception
{
	private:
		size_t userId;
		std::string message;
	public:
		UserCacheException(size_t userId, const std::string& str) throw();
		UserCacheException(const UserCacheException& other) throw();
		~UserCacheException() throw();
		const char *what() const throw();
};

class UserCacheExceptionString : public std::exception
{
	private:
		std::string value;
		std::string message;
	public:
		UserCacheExceptionString(const std::string &value, const std::string& str) throw();
		~UserCacheExceptionString() throw();
		const char *what() const throw();
		const std::string getValue() const throw();
};

#endif
