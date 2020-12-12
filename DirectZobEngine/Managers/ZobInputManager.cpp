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

float MapToOne(float const value, void* userData)
{
	if (value != 0.0f)
	{
		return 1.0f;
	}
	return 0.0f;
}
float MapToMinusOne(float const value, void* userData)
{
	if (value != 0.0f)
	{
		return -1.0f;
	}
	return 0.0f;
}

void ZobInputManager::Map()
{
	m_deadZone = 0.f;
	const gainput::DeviceId padId = m_gainputManager->CreateDevice<gainput::InputDevicePad>(0);
	const gainput::DeviceId keyboardId = m_gainputManager->CreateDevice<gainput::InputDeviceKeyboard>(0);
	const gainput::DeviceId mouseId = m_gainputManager->CreateDevice<gainput::InputDeviceMouse>(0);

	m_map->MapFloat(LeftStickX, keyboardId, gainput::KeyLeft, 0.0f, 1.0f, &MapToMinusOne);
	m_map->MapFloat(LeftStickX, keyboardId, gainput::KeyRight, 0.0f, 1.0f, &MapToOne);

	m_map->MapFloat(LeftStickX, padId, gainput::PadButtonLeftStickX);
	m_map->MapFloat(LeftStickY, padId, gainput::PadButtonLeftStickY);
	m_map->MapFloat(RightStickX, padId, gainput::PadButtonRightStickX);
	m_map->MapFloat(RightStickY, padId, gainput::PadButtonRightStickY);
	m_map->MapFloat(LeftShoulder, padId, gainput::PadButtonAxis4);
	m_map->MapFloat(RightShoulder, padId, gainput::PadButtonAxis5);

	m_map->SetDeadZone(gainput::PadButtonLeftStickX, m_deadZone);
	m_map->SetDeadZone(gainput::PadButtonLeftStickY, m_deadZone);
	m_map->SetDeadZone(gainput::PadButtonRightStickX, m_deadZone);
	m_map->SetDeadZone(gainput::PadButtonRightStickY, m_deadZone);

	m_map->MapFloat(buttonA, padId, gainput::PadButtonA);
}