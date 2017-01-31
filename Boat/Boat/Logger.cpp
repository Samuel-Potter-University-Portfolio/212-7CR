#include "Logger.h"

#include <ctime>
#include <time.h>
#include <iostream>
#include <sstream>


std::string TimeStamp()
{
	time_t time_stamp = time(nullptr);
	tm date;

	gmtime_s(&date, &time_stamp);

	char time_str[80];
	strftime(time_str, 80, "[%d-%m-%Y][%H:%M:%S]", &date);

	return time_str;
}

void Logger::LogMessage(const Verbosity verbosity, const std::string message)
{
	std::stringstream stream;
	std::string time_stamp = TimeStamp();

	if (verbosity == Warning)
		stream << time_stamp << "[__WARNING__]:" << message << '\n';

	else if (verbosity == Error)
		stream << time_stamp << "[__ERROR__]" << message << '\n';

	else if (verbosity == Fatal)
	{
		stream << time_stamp <<"[__FATAL__]" << message << '\n';
		std::cout << stream.str();
		exit(0);
	}
	else
		stream << time_stamp << ':' << message << '\n';

	//Store to temp stream stream to prevent cout for mixing thread messages together
	std::cout << stream.str();
}