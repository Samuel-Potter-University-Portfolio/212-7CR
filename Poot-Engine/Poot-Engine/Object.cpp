#include "Object.h"


Object::Object()
{
	parent = nullptr;
	tags = OBJ_TAG_NONE;
}

Object::~Object()
{
}

void Object::AddTag(Tag tag)
{
	tags |= tag;
}

void Object::SetOwner(Object* object)
{
	this->parent = object;
}

bool Object::HasTag(Tag tag)
{
	if (parent)
		return HasPersonalTag(tag) || parent->HasPersonalTag(tag);
	else
		return HasPersonalTag(tag);
}

bool Object::IsEnabled()
{
	if (parent)
		return IsActive() && parent->IsEnabled();
	else
		return IsActive();
}