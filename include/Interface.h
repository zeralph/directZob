#pragma once
#include "Types.h"
#include "Engine.h"

class __declspec(dllexport) DirectZobInterface
{
public:

	DirectZobInterface(Engine* engine);
	~DirectZobInterface();

	int Test() const;
	const DirectZobType::BufferData* GetBuffer() const;
private:
	Engine* m_engine;
};