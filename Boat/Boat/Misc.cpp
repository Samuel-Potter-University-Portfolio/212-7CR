#include "Misc.h"
#include <Windows.h>


float System::GetSysTimeSeconds()
{
	SYSTEMTIME sys_time;
	GetSystemTime(&sys_time);

	return sys_time.wSecond;
}
float System::GetSysTimeMilli()
{
	SYSTEMTIME sys_time;
	GetSystemTime(&sys_time);

	return sys_time.wMilliseconds;
}