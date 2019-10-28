#pragma once
struct MousePoint
{
	int x, y;
};
class MouseEvent
{
public:
	enum EventType
	{
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		RAW_MOVE,
		Invalid
	};

	MouseEvent();
	MouseEvent(const EventType type, const int x, const int y);
	const bool IsValid();
	const EventType GetType();
	const MousePoint GetPos();
	const int GetPosX();
	const int GetPosY();
private:
	EventType type;
	int x, y;
};

