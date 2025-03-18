#ifndef COLORS_HPP
	#define COLORS_HPP
	#include <ostream>
	class Colors {
	public:
		/**
		 * @brief Enum class for colors
		 * @ref reference https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
		 * @example See the example in the Configuration.cpp file in function printSections(); !
		 **/
		enum Code {
			RESET		= 0,
			BOLD		= 1,
			FG_BLACK	= 30,
			FG_DRED		= 31,
			FG_GREEN	= 32,
			FG_LYELLOW	= 93,
			FG_DYELLOW	= 33,
			FG_BLUE		= 34,
			FG_MAGENTA	= 35,
			FG_CYAN		= 36,
			FG_LGRAY	= 37,
			FG_DGRAY	= 90,
			FG_LRED		= 91,
			FG_DEFAULT	= 39,
		};
		Code code;
		Colors(Code pCode);
	};
	std::ostream& operator<<(std::ostream& os, const Colors& color);
#endif
