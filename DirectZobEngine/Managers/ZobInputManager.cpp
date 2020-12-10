#include "ZobInputManager.h"

ZobInputManager::ZobInputManager(int width, int height)
{
	m_gainputManager.SetDisplaySize(width, height);
	m_map = new gainput::InputMap(m_gainputManager);
	Map();
}

ZobInputManager::~ZobInputManager()
{
	delete m_map;
}

void ZobInputManager::Update()
{
	m_gainputManager.Update();
	if (m_map->GetBoolWasDown(buttonA))
	{
		int y = 0;
		y++;
	}
}

void ZobInputManager::Map()
{
	const gainput::DeviceId padId = m_gainputManager.CreateDevice<gainput::InputDevicePad>(0);
	m_map->MapFloat(buttonA, padId, gainput::PadButtonA);
}