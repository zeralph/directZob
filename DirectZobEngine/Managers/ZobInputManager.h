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
		LeftStickX,
		LeftStickY,
		LeftShoulder,
		RightStickX,
		RightStickY,
		RightShoulder,
		WireFrame,
		Gizmos,
		Quit,
		NextCamera,
		SwitchEqualizeTriangleQueues,
		NextLightMode,
		SwitchBuffers,
		SwitchFPS,
		Handbrake,
		Start,
		Select,
		MenuUp,
		MenuLeft,
		MenuRight,
		MenuDown,
		switchPerspectiveCorrection,
		switchColorDepth,
	};

	ZobInputManager(int width, int height);
	~ZobInputManager();
#ifdef WINDOWS
	void						Update(uint64_t deltaTimeMS, HWND hWnd);
#elif LINUX
	void						Update(uint64_t deltaTimeMS, Display* display);
#else
	void						Update(uint64_t deltaTimeMS);
#endif
	const gainput::InputMap*	GetMap() { return m_map;}
	
	void						Map();
private:
	gainput::InputManager* m_gainputManager;
	gainput::InputMap* m_map;
	float m_deadZone;
};