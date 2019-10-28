#include "MouseEvent.h"

MouseEvent::MouseEvent() : type(EventType::Invalid),x(0),y(0)
{
}

MouseEvent::MouseEvent(const EventType type, const int x, const int y) : type(type), x(x), y(y)
{
}

const bool MouseEvent::IsValid()
{
	return this->type != EventType::Invalid;
}

const MouseEvent::EventType MouseEvent::GetType()
{
	return this->type;
}

const MousePoint MouseEvent::GetPos()
{
	return { this->x, this->y };
}

const int MouseEvent::GetPosX()
{
	return this->x;
}

const int MouseEvent::GetPosY()
{
	return  this->y;
}
