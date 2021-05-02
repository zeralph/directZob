#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "ManagedVector3.h"
#include "../Rendering/Engine.h"
#include "../DirectZob.h"
#include "ZobObjectWrapper.h"
#using "System.Windows.Forms.dll"
#using "System.dll"
#using "System.Drawing.dll"

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Drawing;

#define NB_EDITOR_TRIANGLES 1000
#define NB_EDITOR_LINES 1000
#define NB_EDITOR_CIRCLES 1000
class DirectZobWrapper;
namespace CLI
{
	public struct Line
	{
		ZobVector3 p0;
		ZobVector3 p1;
		int color;
		bool bold;
		bool noZ;
	};
	public struct Circle
	{
		ZobVector3 p;
		ZobVector3 n;
		float r;
		int color;
		bool bold;
		bool noZ;
	};
	public enum objectModificator
	{
		translate_world=0,
		translate_local=1,
		rotate_world=2,
		rotate_local=3,
		scale=4,
	};
	public enum axis
	{
		X=0,
		Y,
		Z,
	};
	public ref class EngineWrapper: public ManagedObject<Core::Engine>
	{
	public:
		EngineWrapper(PictureBox^ renderWindow);
		~EngineWrapper();
		int				GetBufferWidth();
		int				GetBufferHeight();
		void			ShowGrid(bool b) { m_Instance->ShowGrid(b); }
		void			WireFrame(bool b) { m_Instance->WireFrame(b); }
		void			ShowNormals(bool b) { m_Instance->ShowNormals(b); }
		void			DrawGizmos(bool b) { m_Instance->DrawGizmos(b); }
		void			DrawPhysicsGizmos(bool b) { m_Instance->DrawPhysyicsGizmos(b); }
		void			ShowBBoxes(bool b) { m_Instance->ShowBBoxes(b); }
		void			ShowText(bool b) { m_Instance->ShowText(b); }
		void			LockFrustrum(bool b) { m_Instance->LockFrustrum(b); }
		bool			LockFrustrum()	{ return m_Instance->LockFrustrum(); }
		void			DrawCameraGizmos(bool b) { m_Instance->DrawCameraGizmos(b); }
		bool			DrawCameraGizmos() { return m_Instance->DrawCameraGizmos(); }
		void			SetRenderOutput(int r);
		void			SetRenderMode(int r);
		void			SetLightingPrecision(int r);
		void			SetObjectModificator(objectModificator om) { m_objectModificator = om;}
		void			DrawLine(ZobVector3* p0, ZobVector3* p1, int color, bool bold, bool noZ);
		void			DrawCircle(ZobVector3* p0, ZobVector3* up, float r, int color, bool bold, bool noZ);
		void			QueueObjectsToRender();
		void			Update(float dt);
		void			Stop() { m_running = false; }
		String^ test;

	private:
		ZobVector3		ToScreenCoords(ZobVector3& v);
		void			InitGizmos();
		void			UpdateCameraEditor(float dt);
		void			OnMouseWheel(Object^ sender, MouseEventArgs^ e);
		void			OnMouseHover(Object ^ sender, EventArgs^ e);
		void			OnMouseLeave(Object^ sender, EventArgs^ e);
		void			UpdateRenderWindowInternal();
		void			UpdateModificationGizmos();
		void			OnObjectSelected(ZobObjectWrapper^ z);
		void			OnNewScene();
		bool			IsCursorInsideWindow();
		PictureBox^		m_renderWindow;
		Graphics^		m_renderWindowGraphics;
		Triangle*		m_triangleList;
		ZobVector3*		m_vertices;
		ZobVector3*		m_projectedVertices;
		ZobVector3*		m_normals;
		ZobVector2*		m_uvs;
		int				m_nbTriangles;
		int				m_nbLines;
		int				m_nbCircles;
		bool			m_running;
		objectModificator m_objectModificator;
		Label^			m_bCenter;
		Label^			m_bTZ;
		Label^			m_bTY;
		Label^			m_bTX;
		Point			m_mouseCoords;
		bool			m_mouseInside;
	};
}
#endif //_WINDLL