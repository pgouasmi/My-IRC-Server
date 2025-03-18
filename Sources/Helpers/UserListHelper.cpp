#include "UserListHelper.hpp"
#include "PrimitivePredicate.hpp"
#include "IRCPredicate.hpp"
#include <algorithm>


bool UserListHelper::containsTarget(std::list<size_t> list, User &target)
{
	return std::find_if(list.begin(), list.end(), SizeTPredicate(target.getUniqueId())) != list.end();
}

bool UserListHelper::addTargetToList(std::list<size_t> list, User &target)
{
	if (containsTarget(list, target))
		return false;
	list.push_back(target.getUniqueId());
	return true;
}

bool UserListHelper::removeTargetFromList(std::list<size_t> list, User &target)
{
	if (!containsTarget(list, target))
		return false;
	list.remove_if(SizeTPredicate(target.getUniqueId()));
	return true;
}