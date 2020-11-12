#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "ManagedVector3.h"
#include "Engine.h"
#include "DirectZob.h"
#include "ZobObjectWrapper.h"
//using namespace System;

#define NB_EDITOR_TRIANGLES 1000

namespace CLI
{
	public ref class EngineWrapper: public ManagedObject<Core::Engine>
	{
	public:

		EngineWrapper();
		~EngineWrapper();
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
		void			DrawTriangle(ManagedVector3^ p0, ManagedVector3^ p1, ManagedVector3^ p2, int color);
		void			QueueObjectsToRender();
	private:
		//array<int>^ m_buffer;
		Triangle*		m_triangleList;
		ZobVector3*		m_vertices;
		ZobVector3*		m_projectedVertices;
		ZobVector3*		m_normals;
		int				m_nbTriangles;
	};
}
#endif //_WINDLL