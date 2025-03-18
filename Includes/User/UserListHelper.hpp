#ifndef USERLISTHELPER_HPP
#define USERLISTHELPER_HPP
	#include <list>
	#include "User.hpp"

class UserListHelper
{
	static bool containsTarget(std::list<size_t> list, User& target);
	static bool addTargetToList(std::list<size_t> list, User& target);
	static bool removeTargetFromList(std::list<size_t> list, User& target);
};

#endif
