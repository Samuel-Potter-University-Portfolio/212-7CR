#pragma once
#include "API.h"
#include "Tickable.h"


typedef unsigned int Tag;

#define OBJ_TAG_PLAYER		(1<<0)
#define OBJ_TAG_COMPONENT	(1<<2)
#define OBJ_TAG_PROP		(1<<3)
#define OBJ_TAG_ENV			(1<<4)
#define OBJ_TAG_SKYBOX		(1<<5)
#define OBJ_TAG_UI			(1<<6)

#define OBJ_TAG_LAST	OBJ_TAG_UI

#define OBJ_TAG_NONE 0
#define OBJ_TAG_ALL (Tag)(-1)


/*
Serves as a base for all hierarchical classes,
*/
class POOT_API Object : public Tickable
{
private:
	Object* parent;
	Tag tags;

protected:
	inline void SetTags(Tag tag) { tags = tag; };
	void AddTag(Tag tag);

public:
	Object();
	virtual ~Object();
	
	inline Object* GetParent() { return parent; }

	virtual void SetOwner(Object* object);

	inline bool HasPersonalTag(Tag tag) { return (tags & tag) != 0; }
	bool HasTag(Tag tag);

	bool IsEnabled();
};