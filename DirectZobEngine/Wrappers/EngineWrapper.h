#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "ManagedVector3.h"
#include "../Rendering/Engine.h"
#include "../DirectZob.h"
#include "ZobObjectWrapper.h"
//using namespace System;

#define NB_EDITOR_TRIANGLES 1000
#define NB_EDITOR_LINES 1000
#define NB_EDITOR_CIRCLES 1000
namespace CLI
{
	struct Line
	{
		ZobVector3 p0;
		ZobVector3 p1;
		int color;
		bool bold;
		bool noZ;
	};
	struct Circle
	{
		ZobVector3 p;
		ZobVector3 n;
		float r;
		int color;
		bool bold;
		bool noZ;
	};
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
		ZobObjectWrapper^ GetObjectAt2DCoords(float x, float y);
		void			DrawLine(ManagedVector3^ p0, ManagedVector3^ p1, int color, bool bold, bool noZ);
		void			DrawCircle(ManagedVector3^ p0, ManagedVector3^ up, float r, int color, bool bold, bool noZ);
		void			DrawTriangle(ManagedVector3^ p0, ManagedVector3^ p1, ManagedVector3^ p2, int color);
		void			QueueObjectsToRender();
	private:
		Triangle*		m_triangleList;
		ZobVector3*		m_vertices;
		ZobVector3*		m_projectedVertices;
		ZobVector3*		m_normals;
		int				m_nbTriangles;
		int				m_nbLines;
		int				m_nbCircles;
	};
}
#endif //_WINDLL