#include "ZobInputManager.h"

ZobInputManager::ZobInputManager(int width, int height)
{
	m_gainputManager = new gainput::InputManager(false);
	m_gainputManager->SetDisplaySize(width, height);
	m_map = new gainput::InputMap(*m_gainputManager);
	Map();
}

ZobInputManager::~ZobInputManager()
{
	delete m_gainputManager;
	delete m_map;
}

void ZobInputManager::Update(uint64_t tick)
{
	m_gainputManager->Update(tick);
}

void ZobInputManager::Map()
{
	m_deadZone = 0.f;
	const gainput::DeviceId padId = m_gainputManager->CreateDevice<gainput::InputDevicePad>(0);
	m_map->MapFloat(buttonA, padId, gainput::PadButtonA);
	m_map->MapFloat(LeftStickX, padId, gainput::PadButtonLeftStickX);
	m_map->MapFloat(LeftStickY, padId, gainput::PadButtonLeftStickY);
	m_map->MapFloat(RightStickX, padId, gainput::PadButtonRightStickX);
	m_map->MapFloat(RightStickY, padId, gainput::PadButtonRightStickY);
	m_map->SetDeadZone(gainput::PadButtonLeftStickX, m_deadZone);
	m_map->SetDeadZone(gainput::PadButtonLeftStickY, m_deadZone);
	m_map->SetDeadZone(gainput::PadButtonRightStickX, m_deadZone);
	m_map->SetDeadZone(gainput::PadButtonRightStickY, m_deadZone);
}