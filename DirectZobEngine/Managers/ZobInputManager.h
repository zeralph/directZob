#pragma once
#include "../Types.h"
#include <string>
#include <vector>
#include <gainput/gainput.h>

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
	};

	ZobInputManager(int width, int height);
	~ZobInputManager();
#ifdef WINDOWS
	void						Update(uint64_t tick, HWND hWnd);
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