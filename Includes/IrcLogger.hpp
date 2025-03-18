#ifndef ILOGGER_HPP
	#define ILOGGER_HPP
		#include <fstream>
		#include <string>
		#include <map>
	class IrcLogger {
		public:
			enum LogLevel
			{
				TRACE,
				DEBUG,
				INFO,
				WARN,
				ERROR
			};
		private:
			std::ofstream* logFile;
			LogLevel logLevel;
			std::string getTime();
			static IrcLogger* instance;
			std::map<LogLevel, void (IrcLogger::*)(const std::string&)> logFunctions;
			void trace(const std::string& message);
			void debug(const std::string& message);
			void info(const std::string& message);
			void warn(const std::string& message);
			void error(const std::string& message);
		public:
			IrcLogger();
			~IrcLogger();
			void open(const std::string& file);
			void setLogLevel(LogLevel level);
			LogLevel getLogLevelFromString(const std::string &value);
			void close();
			static IrcLogger* getLogger();
			void log(LogLevel level, const std::string& message);
	};


#endif
