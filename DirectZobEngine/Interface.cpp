#include "Interface.h"

Core::Interface::Interface()
{
	m_directZob.Init();
}

Core::Interface::~Interface()
{
}

int Core::Interface::RunAFrame()
{
	return m_directZob.RunAFrame();
}

int Core::Interface::GetBufferWidth()
{
	return m_directZob.GetEngine()->GetBufferData()->width;
}
int Core::Interface::GetBufferHeight()
{
	return m_directZob.GetEngine()->GetBufferData()->height;
}

int* Core::Interface::GetBufferData()
{
	return (int*)m_directZob.GetEngine()->GetBufferData()->buffer;
}

int Core::Interface::Test()
{
	return 42;
}

std::vector<std::string>* Core::Interface::GetEvents()
{
	return m_directZob.GetEvents();
}

void Core::Interface::ClearEvents()
{
	m_directZob.ClearEvents();
}

/*const DirectZobType::BufferData* DirectZob::Cpp::DirectZobInterface::GetBuffer()
{
	return m_engine->GetBufferData();
}*/