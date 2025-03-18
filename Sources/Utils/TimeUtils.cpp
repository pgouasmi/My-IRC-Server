#include "TimeUtils.hpp"
#include <ctime>
#include <unistd.h>

std::string TimeUtils::getCurrentTime()
{
	std::time_t now = std::time(0);
	std::tm* localTime = std::localtime(&now);

	char buffer[255];
	std::strftime(buffer, sizeof(buffer), "%D-%H:%M:%S ", localTime);
	return std::string(buffer);
}

size_t  TimeUtils::getCurrentTimeMillis()
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

size_t  TimeUtils::getTimeMillisAt(size_t time)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000) + time;
}

void sleep(size_t ms)
{
	size_t time = TimeUtils::getTimeMillisAt(ms);
	while (TimeUtils::getCurrentTimeMillis() < time)
		usleep(25);
}


