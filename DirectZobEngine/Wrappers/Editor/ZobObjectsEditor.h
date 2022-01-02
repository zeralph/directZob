#ifdef _WINDLL
#pragma once
#include "../ManagedObject.h"
#include "../../DirectZobEngine/ZobObjects/ZobObject.h"
#include "../../DirectZob.h"
#include <string>
#include <vector>

#define EDITOR_OBJECT "Editor_object"
#define EDITOR_ARROW_X "Editor_arrow_x"
#define EDITOR_ARROW_Y "Editor_arrow_y"
#define EDITOR_ARROW_Z "Editor_arrow_z"
#define EDITOR_ROTATE_X "Editor_rotate_x"
#define EDITOR_ROTATE_Y "Editor_rotate_y"
#define EDITOR_ROTATE_Z "Editor_rotate_z"
using namespace System;
using namespace System::Collections::Generic;

namespace CLI
{
	public class ZobObjectsEditor
	{
	public:

		enum eGizmoModificatorType
		{
			eGizmo_none = 0,
			eGizmo_translate = 1,
			eGizmo_rotate = 2,
			eGizmo_scale = 3,
		};
		enum eGizmoModificatorSpace
		{
			space_none = 0,
			space_local = 1,
			space_world = 2,
		};
		enum eGizmoModificatorAxis
		{
			axis_none = 0,
			axis_x = 1,
			axis_y = 2,
			axis_z = 3,
		};
		enum axis
		{
			X = 0,
			Y,
			Z,
		};
		struct ModificatorData
		{
			//mouse move stuff
			ZobObject*				m_currentObjectModificator;
			ZobObject*				m_currentModifiedObject;
			eGizmoModificatorType	m_objectModificatorType;
			eGizmoModificatorAxis	m_objectModificatorAxis;
			eGizmoModificatorSpace	m_objectModificatorSpace;
			ZobVector3				m_startPosition;
			ZobVector3				m_startOrientation;
			ZobVector3				m_planePosition;
			ZobVector3				m_planeNormal;
			ZobVector3				m_objectDirection;
			ZobVector3				m_objectPosition;
			ZobVector3				m_deltaStart;
			ZobVector3				m_startAxisRotationVector;
			ZobColor				m_color;
			float					m_startAngleRotation;
			float					m_snap;
			bool					m_moveObject;
			void	Reset()
			{
				m_currentObjectModificator = NULL;
				//m_objectModificator = objectModificator::translate_local;
				m_objectModificatorAxis = eGizmoModificatorAxis::axis_none;
				m_moveObject = false;
				m_currentObjectModificator = NULL;
				m_currentModifiedObject = NULL;
				m_startPosition = ZobVector3(0, 0, 0);
				m_startOrientation = ZobVector3(0, 0, 0);
				m_planePosition = ZobVector3(0, 0, 0);
				m_planeNormal = ZobVector3(0, 0, 0);
				m_objectDirection = ZobVector3(0, 0, 0);
				m_objectPosition = ZobVector3(0,0,0);
				m_deltaStart = ZobVector3(0, 0, 0);
				m_color = ZobColor::White;
			}
		};
		ZobObjectsEditor();
		~ZobObjectsEditor();
		void				AddEditorGizmos();
		void				SetParent(ZobObject* z);
		void				Select(ZobObject* z);
		void				UnSelect();
		void				SetWorld();
		void				SetLocal();
		void				Scale(float s);
		void				Show(eGizmoModificatorType type);
		void				Hide();
		ModificatorData*	GetModificatorData() {return &m_modificatorData;}
	private:
		zobId				m_gizmosGuid;
		ZobObject*			m_editorRoot;
		ZobObject*			m_translateX;
		ZobObject*			m_translateY;
		ZobObject*			m_translateZ;
		ZobComponentMesh*	m_behaviorTranslateX;
		ZobComponentMesh*	m_behaviorTranslateY; 
		ZobComponentMesh*	m_behaviorTranslateZ;
		ZobObject*			m_rotateX;
		ZobObject*			m_rotateY;
		ZobObject*			m_rotateZ;
		ZobComponentMesh*	m_behaviorRotateX;
		ZobComponentMesh*	m_behaviorRotateY;
		ZobComponentMesh*	m_behaviorRotateZ;
		ZobObject*			m_currentSelectedGizmo;
		ModificatorData		m_modificatorData;

		static string gArrow;
		static string gCircle;
	};
}
#endif //_WINDLL

