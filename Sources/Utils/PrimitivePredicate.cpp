#include "../../Includes/PrimitivePredicate.hpp"
#include <string>

StringPredicate::StringPredicate(const std::string& v) : target(v) {}
bool StringPredicate::operator () (const std::string& a)
{
	return a == target;
}

bool StringPredicate::operator == (const std::string& a)
{
	return a == target;
}

SizeTPredicate::SizeTPredicate(size_t v) : target(v) {}
bool SizeTPredicate::operator () (size_t a)
{
	return a == target;
}

bool SizeTPredicate::operator == (std::size_t a)
{
	return a == target;
}
