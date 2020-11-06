#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "ManagedVector3.h"
#include "Engine.h"
#include "DirectZob.h"
#include "ZobObjectWrapper.h"
//using namespace System;

namespace CLI
{
	public ref class EngineWrapper: public ManagedObject<Core::Engine>
	{
	public:

		EngineWrapper();
		int				GetBufferWidth();
		int				GetBufferHeight();
		System::IntPtr	GetBufferData();
		void			ShowGrid(bool b) { m_Instance->ShowGrid(b); }
		void			WireFrame(bool b) { m_Instance->WireFrame(b); }
		void			ShowNormals(bool b) { m_Instance->ShowNormals(b); }
		void			DrawGizmos(bool b) { m_Instance->DrawGizmos(b); }
		void			SetRenderOutput(int r);
		void			SetRenderMode(int r);
		void			SetLightingPrecision(int r);
		bool			GetProjectedCoords(ManagedVector3^ worldSpacePos);
		float			GetDistanceToCamera(ManagedVector3^ worldPos);
		ZobObjectWrapper^ GetObjectAt2DCoords(ManagedVector3^ v);
		void			DrawLine(ManagedVector3^ p0, ManagedVector3^ p1, int color);
		void			DrawCircle(ManagedVector3^ p0, ManagedVector3^ up, float r, int color);
	private:
		//array<int>^ m_buffer;
	};
}
#endif //_WINDLL