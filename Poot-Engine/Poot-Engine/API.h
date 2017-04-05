#pragma once

#ifdef _POOT_EXPORT
#    define POOT_API __declspec(dllexport)
#else
#    define POOT_API __declspec(dllimport)
#endif

#define POOT_VERSION_MAJOR 0
#define POOT_VERSION_MINOR 1
#define POOT_VERSION_PATCH 0


#define Super __super


template<class Type, class Class>
inline Type* Cast(Class* object)
{
	return dynamic_cast<Type*>(object);
}