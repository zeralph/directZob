//Entity.h
#pragma once
#include "ManagedObject.h"
#include "../DirectZobEngine/Interface.h"

//using namespace System;
namespace CLI
{
	public ref class InterfaceWrapper : public ManagedObject<Core::Interface>
	{
	public:

		InterfaceWrapper();

		int RunAFrame();
		int GetBufferWidth();
		int GetBufferHeight();
		System::IntPtr GetBufferData();


		int Test();

	private:
		array<int>^ m_buffer;
	};
}