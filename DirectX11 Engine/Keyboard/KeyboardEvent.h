#pragma once
class KeyboardEvent
{
public:
	enum EventType
	{
		Press,
		Release,
		Invalid
	};

	KeyboardEvent();
	KeyboardEvent(const EventType type, const unsigned char key);
	const bool IsPress();
	const bool IsRelease();
	const bool IsValid();
	const unsigned char GetKeyCode();
private:
	EventType type;
	unsigned char key;
};

