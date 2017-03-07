#pragma once

#include <string>
#include <sstream>

#define LOG(verbosity, message, ...) { \
	char f_msg[256]; \
	snprintf(f_msg, sizeof(f_msg), message, __VA_ARGS__); \
	Logger::LogMessage(Logger::verbosity, f_msg); \
}

namespace Logger
{
	enum Verbosity
	{
		Log, Warning, Error, Fatal
	};


	void LogMessage(const Verbosity verbosity, const std::string message);
}
