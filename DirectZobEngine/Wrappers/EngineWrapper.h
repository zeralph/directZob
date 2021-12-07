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
		none = 0,
		translate_world_x = 1,
		translate_world_y = 2,
		translate_world_z = 3,
		translate_local_x = 4,
		translate_local_y = 5,
		translate_local_z = 6,
		rotate_world_x = 7,
		rotate_world_y = 8,
		rotate_world_z = 9,
		rotate_local_x = 10,
		rotate_local_y = 11,
		rotate_local_z = 12,
		scale_x = 13,
		scale_y = 14,
		scale_z = 15,
	};
	public enum axis
	{
		X=0,
		Y,
		Z,
	};
	public ref class EngineWrapper: public ManagedObject<Engine>
	{
	public:
		EngineWrapper(PictureBox^ renderWindow);
		~EngineWrapper();
		int				GetBufferWidth();
		int				GetBufferHeight();
		void			ShowGrid(bool b) { m_Instance->ShowGrid(b); }
		void			WireFrame(bool b) { m_Instance->WireFrame(b); }
		void			ShowNormals(bool b) { m_Instance->ShowNormals(b); }
		void			ToggleZbufferOutput(bool b) { m_Instance->ToggleZbufferOutput(); }
		void			DrawGizmos(bool b) { m_Instance->DrawGizmos(b); }
		void			DrawPhysicsGizmos(bool b) { m_Instance->DrawPhysyicsGizmos(b); }
		void			ShowBBoxes(bool b) { m_Instance->ShowBBoxes(b); }
		void			ShowText(bool b) { m_Instance->ShowText(b); }
		void			LockFrustrum(bool b) { m_Instance->LockFrustrum(b); }
		bool			LockFrustrum()	{ return m_Instance->LockFrustrum(); }
		void			DrawCameraGizmos(bool b) { m_Instance->DrawCameraGizmos(b); }
		bool			DrawCameraGizmos() { return m_Instance->DrawCameraGizmos(); }
		void			SetRenderMode(int r);
		void			SetLightingPrecision(int r);
		void			SetObjectModificator(objectModificator om) { m_objectModificator = om;}
		void			DrawLine(ZobVector3* p0, ZobVector3* p1, int color, bool bold, bool noZ);
		void			DrawCircle(ZobVector3* p0, ZobVector3* up, float r, int color, bool bold, bool noZ);
		void			QueueObjectsToRender();
		void			Update(float dt);
		void			Stop() { m_running = false; }
		void			Resize(int w, int h);
		String^ test;

	private:
		ZobVector3		ToScreenCoords(ZobVector3& v);
		void			UpdateCameraEditor(float dt);
		void			OnMouseWheel(Object^ sender, MouseEventArgs^ e);
		void			OnMouseHover(Object ^ sender, EventArgs^ e);
		void			OnMouseLeave(Object^ sender, EventArgs^ e);
		void			OnMouseClick(Object^ sender, MouseEventArgs^ e);
		void			OnMouseDown(Object^ sender, MouseEventArgs^ e);
		void			OnMouseUp(Object^ sender, MouseEventArgs^ e);
		void			OnMouseMove(Object^ sender, MouseEventArgs^ e);
		void			UpdateRenderWindowInternal();
		void			OnObjectSelected(ZobObjectWrapper^ z);
		void			OnNewScene();
		bool			IsCursorInsideWindow();
		void			UpdateMoveObject();
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
		Point			m_mouseCoords;
		Point			m_mouseLastCoords;
		bool			m_mouseInside;

		//mouse move stuff
		ZobObject*		m_currentModifiedObject;
		ZobObject*		m_currentObjectModificator;
		float			m_currentModifiedStartX;
		float			m_currentModifiedStartY;
		float			m_currentModifiedStartZ;
		float			m_lastMouseX;
		float			m_lastMouseY;
		bool			m_moveObject;
	};
}
#endif //_WINDLL