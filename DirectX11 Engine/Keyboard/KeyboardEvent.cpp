#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent() : type(EventType::Invalid), key(0u)
{
}

KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key) : type(type), key(key)
{
}

const bool KeyboardEvent::IsPress()
{
	return this->type == EventType::Press;
}

const bool KeyboardEvent::IsRelease()
{
	return this->type == EventType::Release;
}

const bool KeyboardEvent::IsValid()
{
	return this->type != EventType::Invalid;
}

const unsigned char KeyboardEvent::GetKeyCode()
{
	return this->key;
}