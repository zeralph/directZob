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
#ifdef WINDOWS
void ZobInputManager::Update(uint64_t deltaTimeMS, HWND hWnd)
{
	MSG msg;
	int i = 0;
	while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		m_gainputManager->HandleMessage(msg);
		i++;
	}
	m_gainputManager->Update(deltaTimeMS);
}
#elif LINUX
void ZobInputManager::Update(uint64_t deltaTimeMS, Display* display)
{
	m_gainputManager->Update(deltaTimeMS);
	if(display)
	{
		XEvent event;
		while (XPending(display))
		{
			XNextEvent(display, &event);
			m_gainputManager->HandleEvent(event);
		}
	}
}
#else
void ZobInputManager::Update(uint64_t deltaTimeMS)
{
	m_gainputManager->Update(deltaTimeMS);
}
#endif


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
	const gainput::InputDevicePad* pad = m_gainputManager->CreateAndGetDevice<gainput::InputDevicePad>(0, gainput::InputDevice::DV_RAW);
	const gainput::DeviceId keyboardId = m_gainputManager->CreateDevice<gainput::InputDeviceKeyboard>(0);
	const gainput::DeviceId mouseId = m_gainputManager->CreateDevice<gainput::InputDeviceMouse>(0);

	m_map->MapFloat(LeftStickX, keyboardId, gainput::KeyLeft, 0.0f, 1.0f, &MapToMinusOne);
	m_map->MapFloat(LeftStickX, keyboardId, gainput::KeyRight, 0.0f, 1.0f, &MapToOne);
	m_map->MapFloat(LeftShoulder, keyboardId, gainput::KeyDown, 0.0f, 1.0f, &MapToOne);
	m_map->MapFloat(RightShoulder, keyboardId, gainput::KeyUp, 0.0f, 1.0f, &MapToOne);

	m_map->MapBool(MenuUp, keyboardId, gainput::KeyUp);
	m_map->MapBool(MenuLeft, keyboardId, gainput::KeyLeft);
	m_map->MapBool(MenuRight, keyboardId, gainput::KeyRight);
	m_map->MapBool(MenuDown, keyboardId, gainput::KeyDown);

	m_map->MapFloat(RightStickX, keyboardId, gainput::Key8);
	m_map->MapFloat(RightStickY, keyboardId, gainput::Key4);
	m_map->MapFloat(LeftStickY, keyboardId, gainput::Key6);
	m_map->MapBool(Handbrake, keyboardId, gainput::KeySpace);
	m_map->MapBool(WireFrame, keyboardId, gainput::KeyW);
	m_map->MapBool(Gizmos, keyboardId, gainput::KeyD);
	m_map->MapBool(Quit, keyboardId, gainput::KeyEscape);
	m_map->MapBool(NextCamera, keyboardId, gainput::KeyC);
	m_map->MapBool(Start, keyboardId, gainput::KeyReturn);
	m_map->MapBool(Select, keyboardId, gainput::KeyShiftR | gainput::KeyShiftR);

	//m_map->MapBool(switchRayTracing, keyboardId, gainput::KeyF2);
	m_map->MapBool(SwitchWobbleFactor, keyboardId, gainput::KeyF2);
	m_map->MapBool(switchColorDepth, keyboardId, gainput::KeyF3);
	m_map->MapBool(SwitchFPS, keyboardId, gainput::KeyF4);
	m_map->MapBool(WireFrame, keyboardId, gainput::KeyF5);
	m_map->MapBool(Gizmos, keyboardId, gainput::KeyF6);
	m_map->MapBool(SwitchEqualizeTriangleQueues, keyboardId, gainput::KeyF7);
	m_map->MapBool(NextLightMode, keyboardId, gainput::KeyF8);
	m_map->MapBool(SwitchBuffers, keyboardId, gainput::KeyF9);
	m_map->MapBool(switchPerspectiveCorrection, keyboardId, gainput::KeyF10);
	m_map->MapBool(switchDebugCulling, keyboardId, gainput::KeyF11);
	
	
#ifndef LINUX	//pad does not work well on Linux yet
	gainput::InputDevice::DeviceState s = pad->GetState();
	if(pad )//&& pad->IsAvailable())
	{
		const gainput::DeviceId padId = pad->GetDeviceId();
		m_map->MapFloat(LeftStickX, padId, gainput::PadButtonLeftStickX);
		m_map->MapFloat(LeftStickY, padId, gainput::PadButtonLeftStickY);
		m_map->MapFloat(RightStickX, padId, gainput::PadButtonRightStickX);
		m_map->MapFloat(RightStickY, padId, gainput::PadButtonRightStickY);
		m_map->MapFloat(LeftShoulder, padId, gainput::PadButtonAxis4);
		m_map->MapFloat(RightShoulder, padId, gainput::PadButtonAxis5);
		m_map->MapBool(Start, padId, gainput::PadButtonStart);
		m_map->MapBool(Select, padId, gainput::PadButtonSelect);
		m_map->SetDeadZone(gainput::PadButtonLeftStickX, m_deadZone);
		m_map->SetDeadZone(gainput::PadButtonLeftStickY, m_deadZone);
		m_map->SetDeadZone(gainput::PadButtonRightStickX, m_deadZone);
		m_map->SetDeadZone(gainput::PadButtonRightStickY, m_deadZone);
	
		m_map->MapBool(Handbrake, padId, gainput::PadButtonA);
		m_map->MapBool(NextCamera, padId, gainput::PadButtonB);		
		m_map->MapBool(Gizmos, padId, gainput::PadButtonX);

		m_map->MapBool(WireFrame, padId, gainput::PadButtonL1);
		m_map->MapBool(switchPerspectiveCorrection, padId, gainput::PadButtonR1);
		//m_map->MapBool(switchColorDepth, padId, gainput::PadButtonL2);
		//m_map->MapBool(SwitchWobbleFactor, padId, gainput::PadButtonR2);

		m_map->MapBool(MenuUp, padId, gainput::PadButtonUp);
		m_map->MapBool(MenuLeft, padId, gainput::PadButtonLeft);
		m_map->MapBool(MenuRight, padId, gainput::PadButtonRight);
		m_map->MapBool(MenuDown, padId, gainput::PadButtonDown);
		//m_map->MapBool(buttonY, padId, gainput::PadButtonY);
	}
#endif //LINUX
}