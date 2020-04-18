#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "Engine.h"
#include "DirectZob.h"
//using namespace System;
namespace CLI
{
	public ref class EngineWrapper: public ManagedObject<Core::Engine>
	{
	public:

		EngineWrapper();
		int GetBufferWidth();
		int GetBufferHeight();
		System::IntPtr GetBufferData();
		void ShowGrid(bool b) { m_Instance->ShowGrid(b); }
		void WireFrame(bool b) { m_Instance->WireFrame(b); }
		void SetRenderOutput(int r);
		void Resize(int w, int h);
	private:
		//array<int>^ m_buffer;
	};
}
#endif //_WINDLL