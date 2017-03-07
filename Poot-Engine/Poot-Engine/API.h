#pragma once

#ifdef _POOT_EXPORT
#    define POOT_API __declspec(dllexport)
#else
#    define POOT_API __declspec(dllimport)
#endif

#define POOT_VERSION_MAJOR 0
#define POOT_VERSION_MINOR 0
#define POOT_VERSION_PATCH 1

#define Super __super