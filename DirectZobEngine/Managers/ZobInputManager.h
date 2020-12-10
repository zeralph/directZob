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
	};

	ZobInputManager(int width, int height);
	~ZobInputManager();

	void			Update();
	void			Map();
private:
	gainput::InputManager m_gainputManager;
	gainput::InputMap* m_map;
};