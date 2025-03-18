#ifndef TIMEUTILS_HPP
	#define TIMEUTILS_HPP
	#include <string>
	#include <stddef.h>
	class TimeUtils
	{
		public:
			static std::string getCurrentTime();
			static size_t getCurrentTimeMillis();
			static size_t getTimeMillisAt(size_t time);
			static void sleep(size_t ms);
	};

#endif
