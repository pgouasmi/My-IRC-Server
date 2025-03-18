//
// Created by pgouasmi on 4/12/24.
//

#include "NumericReplies.hpp"

#include <IrcLogger.hpp>
#include <StringUtils.hpp>

void sendServerReply(int const fd, std::string buffer, int color, std::string mode)
{
	if (color != -1)
	{
		std::string scolor = "\x03";
		scolor.append(StringUtils::ltos(color));
		buffer.insert(buffer.find(':', 2) + 1, scolor);
	}
	if (!mode.empty())
	{
		buffer.insert(buffer.find(':', 2) + 1, mode);
		buffer.insert(buffer.length() - 2, mode);
	}
	if (send(fd, buffer.c_str(), buffer.length(), 0) <= 0)
		IrcLogger::getLogger()->log(IrcLogger::WARN, "KO send of " + buffer);
}
