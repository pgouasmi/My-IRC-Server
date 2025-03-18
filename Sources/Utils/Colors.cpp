#include "Colors.hpp"

Colors::Colors(Code pCode) : code(pCode) {}

std::ostream &operator<<(std::ostream &os, const Colors &color)
{
	return os << "\033[" << color.code << "m";
}