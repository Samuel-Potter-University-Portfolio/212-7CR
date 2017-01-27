#include "Logger.h"

#include <ctime>
#include <time.h>
#include <iostream>


std::string TimeStamp()
{
	time_t time_stamp = time(nullptr);
	tm date;

	gmtime_s(&date, &time_stamp);

	char time_str[80];
	strftime(time_str, 80, "[%d\\%m\\%Y][%I:%M:%S]", &date);

	return time_str;
}

void Logger::LogMessage(const Verbosity verbosity, const std::string message)
{
	std::string time_stamp = TimeStamp();

	if (verbosity == Warning)
		std::cout << time_stamp << ":Warning:" << message << '\n';

	if (verbosity == Error)
		std::cout << time_stamp << ":Error:" << message << '\n';

	if (verbosity == Fatal)
	{
		std::cout << time_stamp <<":Fatal:" << message << '\n';
		exit(0);
	}
	else
		std::cout << time_stamp << ':' << message << '\n';
}