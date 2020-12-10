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
		RightStickX,
		RightStickY,
	};

	ZobInputManager(int width, int height);
	~ZobInputManager();
	
	const gainput::InputMap*	GetMap() { return m_map;}
	void						Update();
	void						Map();
private:
	gainput::InputManager m_gainputManager;
	gainput::InputMap* m_map;
};