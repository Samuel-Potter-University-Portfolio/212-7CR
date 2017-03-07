#pragma once

#ifdef _POOT_EXPORT
#    define POOT_API __declspec(dllexport)
#else
#    define POOT_API __declspec(dllimport)
#endif