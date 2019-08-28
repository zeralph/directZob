#include "Interface.h"

DirectZobInterface::DirectZobInterface(Engine* engine)
{
	m_engine = engine;
}

DirectZobInterface::~DirectZobInterface()
{
	m_engine = NULL;
}

int DirectZobInterface::Test() const
{
	return 42;
}

const DirectZobType::BufferData* DirectZobInterface::GetBuffer() const
{
	return m_engine->GetBufferData();
}