#pragma once
#include <wx/event.h>
#include "../DirectZobEngine/DirectZob.h"
#include "../DirectZobEngine/Managers/CameraManager.h"

#define EDITOR_ROOT_NODE "Editor_root"
#define EDITOR_GIZMOS_NODE "Editor_gizmos"
#define EDITOR_ARROW_X "Editor_arrow_x"
#define EDITOR_ARROW_Y "Editor_arrow_y"
#define EDITOR_ARROW_Z "Editor_arrow_z"
#define EDITOR_ROTATE_X "Editor_rotate_x"
#define EDITOR_ROTATE_Y "Editor_rotate_y"
#define EDITOR_ROTATE_Z "Editor_rotate_z"

class ZobMaterial;
class Texture;
class ZobEditorManager
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
		ZobObject* m_currentObjectModificator;
		ZobObject* m_currentModifiedObject;
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
			m_objectPosition = ZobVector3(0, 0, 0);
			m_deltaStart = ZobVector3(0, 0, 0);
			m_color = ZobColor::White;
			m_snap = 0.0f;
		}
	};

public:
	ZobEditorManager(); 
	~ZobEditorManager();

	void				UpdateInterface();
	void				UpdateEngine();
	void				UpdateLog();
	void				OnNewScene();
	void				Unload();
	void				DrawGrid();
	void				UpdateGizmos();
	void				SetSelectedObject(ZobObject* z);
	ZobObject*			GetSelectedObject() { return m_selectedObject; }
	void				SetLastMouseEvent(wxMouseEvent& e);
	void				OnMouseDown(wxMouseEvent& event);
	void				OnMouseUp(wxMouseEvent& event);
	void				OnMouseClick(wxMouseEvent& event);
	void				OnKeyDown(wxKeyEvent& event);
	void				SetObjectModificator(eGizmoModificatorType type, eGizmoModificatorSpace space);

	void				CreateLight();
	void				CreateCamera();
	void				LoadMesh(std::string& p, std::string& f);
	void				AddObject();
	void				ShowHide();
	void				LoadSprite(std::string& p, std::string& f);
	void				Duplicate();
	void				ZoomToSelected();

private:

	void				UpdateView();
	void				UpdateMoveObject();
	void				UpdateGizmoSelection(ZobObject* z);
	void				SetupObjectModificator();
	void				ShowGizmos();
	void				HideGizmos();
	void				ScaleGizmos(float s);
	ModificatorData		m_modificatorData;
	wxMouseEvent		m_lastMouseEvent;
	wxKeyEvent			m_lastKeyEvent;
	ZobObject*			m_selectedObject;
	ZobObject*			m_currentSelectedGizmo;
	ZobObject*			m_editorRootNode;
	ZobObject*			m_gizmosNode;
	ZobObject*			m_translateX;
	ZobComponentMesh*	m_componentTranslateX;
	ZobObject*			m_translateY;
	ZobComponentMesh*	m_componentTranslateY;
	ZobObject*			m_translateZ;
	ZobComponentMesh*	m_componentTranslateZ;
	ZobObject*			m_rotateX;
	ZobComponentMesh*	m_componentRotateX;
	ZobObject*			m_rotateY;
	ZobComponentMesh*	m_componentRotateY;
	ZobObject*			m_rotateZ;
	ZobComponentMesh*	m_componentRotateZ;
	float				m_mouseX;
	float				m_mouseY;
	float				m_mouseDx;
	float				m_mouseDy;
	static char*		gArrow;
	static long			gArrowLen;
	static char*		gCircle;
	static long			gCircleLen;
	static unsigned long			gLight2[];
	static long			gLight2Len;
	static unsigned long			gCamera[];
	static long			gCameraLen;
	Texture*			m_lightImage;
	const ZobMaterial*	m_lightMaterial;
	Texture*			m_cameraImage;
	const ZobMaterial*	m_cameraMaterial;
	bool				m_init;
	std::string 		m_logTmpData;
};