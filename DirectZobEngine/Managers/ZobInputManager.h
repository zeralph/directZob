#pragma once
#include "../Types.h"
#include <string>
#include <vector>
#include <gainput/gainput.h>
#ifdef LINUX
#include <X11/Xlib.h>
#endif
class ZobInputManager
{
public:

	enum Button
	{
		buttonA,
		buttonB,
		buttonX,
		buttonY,
		LeftStickX,
		LeftStickY,
		LeftShoulder,
		RightStickX,
		RightStickY,
		RightShoulder,
		WireFrame,
		Gizmos,
	};

	ZobInputManager(int width, int height);
	~ZobInputManager();
#ifdef WINDOWS
	void						Update(uint64_t tick, HWND hWnd);
#elif LINUX
	void						Update(uint64_t tick, Display* display);
#else
	void						Update(uint64_t tick);
#endif
	const gainput::InputMap*	GetMap() { return m_map;}
	
	void						Map();
private:
	gainput::InputManager* m_gainputManager;
	gainput::InputMap* m_map;
	float m_deadZone;
};