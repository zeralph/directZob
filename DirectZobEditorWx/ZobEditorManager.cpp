#include "ZobEditorManager.h"
#include "MainWindowInterface.h"
#include "../DirectZobEngine/Misc/ZobGeometryHelper.h"
#include "Inspector.h"

ZobEditorManager::ZobEditorManager()
{
	m_init = false;
	m_currentSelectedGizmo = NULL;
	m_selectedObject = NULL;
	m_modificatorData.Reset();
	m_modificatorData.m_objectModificatorType = eGizmoModificatorType::eGizmo_translate;
	m_modificatorData.m_objectModificatorSpace = eGizmoModificatorSpace::space_local;
}

ZobEditorManager::~ZobEditorManager()
{
}

void ZobEditorManager::UpdateInterface()
{
	MainWindowInterface::UpdateControls();	
}

void ZobEditorManager::Unload()
{
	m_init = false;
	m_currentSelectedGizmo = NULL;
	m_selectedObject = NULL;
	m_modificatorData.Reset();
}

void ZobEditorManager::UpdateEngine()
{
	if (m_init)
	{
		UpdateMoveObject();
		UpdateView();
		DrawGrid();
		UpdateGizmos();
		ZobObject* z = DirectZob::GetInstance()->GetEngine()->GetObjectAt2DCoords(m_mouseX, m_mouseY, true);
		UpdateGizmoSelection(z);
		UpdateLog();
	}
}


void ZobEditorManager::OnMouseClick(wxMouseEvent& event)
{
	ZobObject* z = DirectZob::GetInstance()->GetEngine()->GetObjectAt2DCoords(event.GetX(), event.GetY(), false);
	if (z && z != m_selectedObject)
	{
		if (z->IsEditorObject())
		{
			//DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->SelectObject(z);
		}
		else
		{
			SetSelectedObject(z);
		}
	}
	else
	{
		SetSelectedObject(NULL);
	}
	MainWindowInterface::UpdateTreeView();
}

void ZobEditorManager::OnMouseUp(wxMouseEvent& event)
{
	m_modificatorData.m_moveObject = false;
	m_modificatorData.Reset();
}

void ZobEditorManager::SetObjectModificator(eGizmoModificatorType type, eGizmoModificatorSpace space)
{
	m_modificatorData.m_objectModificatorType = type;
	m_modificatorData.m_objectModificatorSpace = space;
	if (space == ZobEditorManager::space_world)
	{
		m_gizmosNode->SetWorldRotation(0, 0, 0);
	}
	else
	{
		m_gizmosNode->SetLocalRotation(0, 0, 0, false);
	}
	SetupObjectModificator();
	ShowGizmos();
}

void ZobEditorManager::UpdateMoveObject()
{
	ZobObject* curObj = m_selectedObject;
	Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	
	if (curObj && m_modificatorData.m_currentObjectModificator && m_modificatorData.m_moveObject)
	{
		if (m_modificatorData.m_objectModificatorType == eGizmoModificatorType::eGizmo_translate)
		{
			BufferData* bData = DirectZob::GetInstance()->GetEngine()->GetBufferData();
			float fx = (float)m_mouseX;
			float fy = (float)m_mouseY;
			fx /= (float)bData->width;
			fy /= (float)bData->height;
			fx = fx * 2.0f - 1.0f;
			fy = fy * 2.0f - 1.0f;
			ZobVector3 ret = c->From2DToWorldOnPlane(fx, fy, &m_modificatorData.m_planePosition, &m_modificatorData.m_planeNormal);
			ret = ZobGeometryHelper::ProjectPointOnLine(&m_modificatorData.m_objectPosition, &m_modificatorData.m_objectDirection, &ret);
			ret = ret - m_modificatorData.m_deltaStart;
			if (m_modificatorData.m_snap != 0)
			{
				ret.x = (int)(ret.x / m_modificatorData.m_snap) * m_modificatorData.m_snap;
				ret.y = (int)(ret.y / m_modificatorData.m_snap) * m_modificatorData.m_snap;
				ret.z = (int)(ret.z / m_modificatorData.m_snap) * m_modificatorData.m_snap;
			}
			if (ret.isNaN())
			{
				return;
			}
			curObj->SetWorldPosition(ret.x, ret.y, ret.z);
		}
		else if (m_modificatorData.m_objectModificatorType == eGizmoModificatorType::eGizmo_rotate)
		{
			BufferData* bData = DirectZob::GetInstance()->GetEngine()->GetBufferData();
			float fx = (float)m_mouseX;
			float fy = (float)m_mouseY;
			fx /= (float)bData->width;
			fy /= (float)bData->height;
			fx = fx * 2.0f - 1.0f;
			fy = fy * 2.0f - 1.0f;

			ZobVector3 ret = c->From2DToWorldOnPlane(fx, fy, &m_modificatorData.m_planePosition, &m_modificatorData.m_planeNormal);
			ZobVector3 st = m_modificatorData.m_deltaStart;
			DirectZob::GetInstance()->GetEngine()->QueueLine(c, &m_modificatorData.m_objectPosition, &st, &ZobColor::Yellow, false, true);
			DirectZob::GetInstance()->GetEngine()->QueueLine(c, &m_modificatorData.m_objectPosition, &ret, &ZobColor::Yellow, false, true);
			ZobVector3 a = st - m_modificatorData.m_objectPosition;
			ZobVector3 b = ret - m_modificatorData.m_objectPosition;
			a.Normalize();
			b.Normalize();
			float d = ZobVector3::Dot(&a, &b);
			//d = fabsf(d);
			ZobVector3 cr = ZobVector3::Cross(&a, &b);
			cr.Normalize();
			float s = ZobVector3::Dot(&m_modificatorData.m_planeNormal, &cr);
			//s = s / fabsf(s);
			if (isnan(s))
			{
				return;
			}
			DirectZob::LogInfo("d : %f - %f", d, s);
			if (d <= 1.0f)
			{
				d = acosf(d) * s;
				if (m_modificatorData.m_snap != 0)
				{
					float rad = DEG_TO_RAD(m_modificatorData.m_snap);
					d = (int)(d / rad) * rad;
				}
				assert(!m_modificatorData.m_startAxisRotationVector.isNaN());
				assert(!isnan(d));
				if (m_modificatorData.m_objectModificatorAxis == eGizmoModificatorAxis::axis_x)
				{
					curObj->SetLocalOrientation(&m_modificatorData.m_startAxisRotationVector, m_modificatorData.m_startAngleRotation, 0);
					curObj->SetLocalOrientation(&ZobVector3::Vector3X, d, 1);
				}
				else  if (m_modificatorData.m_objectModificatorAxis == eGizmoModificatorAxis::axis_y)
				{
					curObj->SetLocalOrientation(&m_modificatorData.m_startAxisRotationVector, m_modificatorData.m_startAngleRotation, 0);
					curObj->SetLocalOrientation(&ZobVector3::Vector3Y, d, 1);
				}
				else  if (m_modificatorData.m_objectModificatorAxis == eGizmoModificatorAxis::axis_z)
				{
					curObj->SetLocalOrientation(&m_modificatorData.m_startAxisRotationVector, m_modificatorData.m_startAngleRotation, 0);
					curObj->SetLocalOrientation(&ZobVector3::Vector3Z, d, 1);
				}
			}
		}
	}
	ZobVector3 va;
	ZobVector3 vb;
	if (m_modificatorData.m_objectModificatorType == eGizmoModificatorType::eGizmo_translate)
	{
		va = m_modificatorData.m_objectDirection;
		vb = m_modificatorData.m_objectDirection;
	}
	else if (m_modificatorData.m_objectModificatorType == eGizmoModificatorType::eGizmo_rotate)
	{
		va = m_modificatorData.m_planeNormal;
		vb = m_modificatorData.m_planeNormal;
	}
	float f = 500.0f;
	va.Mul(-f);
	va.Add(&m_modificatorData.m_objectPosition);
	vb.Mul(f);
	vb.Add(&m_modificatorData.m_objectPosition);
	DirectZob::GetInstance()->GetEngine()->QueueLine(c, &va, &vb, &m_modificatorData.m_color, false, true);
}

void ZobEditorManager::OnMouseDown(wxMouseEvent& event)
{
	m_mouseDx = (float)event.GetX();
	m_mouseDy = (float)event.GetY();
	if (event.ButtonIsDown(wxMouseButton::wxMOUSE_BTN_LEFT))
	{
		if (!m_modificatorData.m_moveObject && m_selectedObject)
		{			
			ZobObject* z = DirectZob::GetInstance()->GetEngine()->GetObjectAt2DCoords(event.GetX(), event.GetY(), true);
			if (z)
			{
				if (z->IsEditorObject())
				{
					DirectZob::LogInfo("mouse down left btn, modificator selected");
					m_modificatorData.m_moveObject = true;
					ZobObject* pz = z->GetParent();
					m_modificatorData.m_currentObjectModificator = z;
					if (pz && pz->GetName() == EDITOR_GIZMOS_NODE)
					{
						std:string s = z->GetName();
						if (s == EDITOR_ARROW_X || s == EDITOR_ROTATE_X)
						{
							m_modificatorData.m_objectModificatorAxis = eGizmoModificatorAxis::axis_x;
						}
						else if (s == EDITOR_ARROW_Y || s == EDITOR_ROTATE_Y)
						{
							m_modificatorData.m_objectModificatorAxis = eGizmoModificatorAxis::axis_y;
						}
						else if (s == EDITOR_ARROW_Z || s == EDITOR_ROTATE_Z)
						{
							m_modificatorData.m_objectModificatorAxis = eGizmoModificatorAxis::axis_z;
						}
						SetupObjectModificator();
					}
				}
			}
		}
	}
}

void ZobEditorManager::ShowGizmos()
{
	HideGizmos();
	if (m_modificatorData.m_objectModificatorType == eGizmo_none)
	{
	}
	else if (m_selectedObject && m_modificatorData.m_objectModificatorType == eGizmo_translate)
	{
		m_componentTranslateX->SetVisible(true);
		m_componentTranslateY->SetVisible(true);
		m_componentTranslateZ->SetVisible(true);
	}
	else if (m_selectedObject && m_modificatorData.m_objectModificatorType == eGizmo_rotate)
	{
		m_componentRotateX->SetVisible(true);
		m_componentRotateY->SetVisible(true);
		m_componentRotateZ->SetVisible(true);
	}
	else //scale
	{

	}
}

void ZobEditorManager::HideGizmos()
{
	m_componentTranslateX->SetVisible(false);
	m_componentTranslateY->SetVisible(false);
	m_componentTranslateZ->SetVisible(false);
	m_componentRotateX->SetVisible(false);
	m_componentRotateY->SetVisible(false);
	m_componentRotateZ->SetVisible(false);
	ScaleGizmos(1);
}

void ZobEditorManager::SetupObjectModificator()
{
	Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	if (m_selectedObject && m_modificatorData.m_currentObjectModificator && m_modificatorData.m_moveObject)
	{
		m_modificatorData.m_currentModifiedObject = m_selectedObject;
		m_modificatorData.m_objectPosition = m_selectedObject->GetWorldPosition();
		m_modificatorData.m_planePosition = m_selectedObject->GetWorldPosition();
		m_modificatorData.m_startOrientation = m_selectedObject->GetLocalRotation();
		if (m_modificatorData.m_objectModificatorType == eGizmoModificatorType::eGizmo_translate)
		{
			if (m_modificatorData.m_objectModificatorAxis == eGizmoModificatorAxis::axis_x)
			{
				if (m_modificatorData.m_objectModificatorSpace == eGizmoModificatorSpace::space_local)
				{
					m_modificatorData.m_planeNormal = m_selectedObject->GetUp();
					m_modificatorData.m_objectDirection = m_selectedObject->GetLeft();
				}
				else
				{
					m_modificatorData.m_planeNormal = ZobVector3::Vector3Y;
					m_modificatorData.m_objectDirection = ZobVector3::Vector3X;
				}
			}
			else if (m_modificatorData.m_objectModificatorAxis == eGizmoModificatorAxis::axis_y)
			{
				if (m_modificatorData.m_objectModificatorSpace == eGizmoModificatorSpace::space_local)
				{
					m_modificatorData.m_planeNormal = m_selectedObject->GetLeft();
					m_modificatorData.m_objectDirection = m_selectedObject->GetUp();
				}
				else
				{
					m_modificatorData.m_planeNormal = ZobVector3::Vector3Z;
					m_modificatorData.m_objectDirection = ZobVector3::Vector3Y;
				}
			}
			else if (m_modificatorData.m_objectModificatorAxis == eGizmoModificatorAxis::axis_z)
			{
				if (m_modificatorData.m_objectModificatorSpace == eGizmoModificatorSpace::space_local)
				{
					m_modificatorData.m_planeNormal = m_selectedObject->GetLeft();
					m_modificatorData.m_objectDirection = m_selectedObject->GetForward();
				}
				else
				{
					m_modificatorData.m_planeNormal = ZobVector3::Vector3X;
					m_modificatorData.m_objectDirection = ZobVector3::Vector3Z;
				}
			}
			BufferData* bData = DirectZob::GetInstance()->GetEngine()->GetBufferData();
			float fx = (float)m_mouseX;
			float fy = (float)m_mouseY;
			fx /= (float)bData->width;
			fy /= (float)bData->height;
			fx = fx * 2.0f - 1.0f;
			fy = fy * 2.0f - 1.0f;
			ZobVector3 ret = c->From2DToWorldOnPlane(fx, fy, &m_modificatorData.m_planePosition, &m_modificatorData.m_planeNormal);
			ret = ZobGeometryHelper::ProjectPointOnLine(&m_modificatorData.m_objectPosition, &m_modificatorData.m_objectDirection, &ret);
			ret = ret - m_modificatorData.m_objectPosition;
			//float d = ret.sqrtLength();
			m_modificatorData.m_deltaStart = ret;// m_modificatorData.m_objectDirection* d;
		}
		else if (m_modificatorData.m_objectModificatorType == eGizmoModificatorType::eGizmo_rotate)
		{
			if (m_modificatorData.m_objectModificatorAxis == eGizmoModificatorAxis::axis_x)
			{
				if (m_modificatorData.m_objectModificatorSpace == eGizmoModificatorSpace::space_local)
				{
					m_modificatorData.m_planeNormal = m_selectedObject->GetLeft();
				}
				else
				{
					m_modificatorData.m_planeNormal = ZobVector3::Vector3X;
				}
			}
			else if (m_modificatorData.m_objectModificatorAxis == eGizmoModificatorAxis::axis_y)
			{
				if (m_modificatorData.m_objectModificatorSpace == eGizmoModificatorSpace::space_local)
				{
					m_modificatorData.m_planeNormal = m_selectedObject->GetUp();
				}
				else
				{
					m_modificatorData.m_planeNormal = ZobVector3::Vector3Y;
				}
			}
			else if (m_modificatorData.m_objectModificatorAxis == eGizmoModificatorAxis::axis_z)
			{
				if (m_modificatorData.m_objectModificatorSpace == eGizmoModificatorSpace::space_local)
				{
					m_modificatorData.m_planeNormal = m_selectedObject->GetForward();
				}
				else
				{
					m_modificatorData.m_planeNormal = ZobVector3::Vector3Z;
				}
			}
			m_selectedObject->GetLocalAxisAngleRotation(m_modificatorData.m_startAxisRotationVector, m_modificatorData.m_startAngleRotation);
			assert(!m_modificatorData.m_startAxisRotationVector.isNaN());
			assert(!isnan(m_modificatorData.m_startAngleRotation));
			BufferData* bData = DirectZob::GetInstance()->GetEngine()->GetBufferData();
			float fx = (float)m_mouseX;
			float fy = (float)m_mouseY;
			fx /= (float)bData->width;
			fy /= (float)bData->height;
			fx = fx * 2.0f - 1.0f;
			fy = fy * 2.0f - 1.0f;
			ZobVector3 ret = c->From2DToWorldOnPlane(fx, fy, &m_modificatorData.m_planePosition, &m_modificatorData.m_planeNormal);
			m_modificatorData.m_deltaStart = ret;
			assert(!ret.isNaN());
		}
		if (m_modificatorData.m_objectModificatorAxis == eGizmoModificatorAxis::axis_x)
		{
			m_modificatorData.m_color = ZobColor::Red;
		}
		else if (m_modificatorData.m_objectModificatorAxis == eGizmoModificatorAxis::axis_y)
		{
			m_modificatorData.m_color = ZobColor::Green;
		}
		else if (m_modificatorData.m_objectModificatorAxis == eGizmoModificatorAxis::axis_z)
		{
			m_modificatorData.m_color = ZobColor::Blue;
		}
	}
}

void ZobEditorManager::UpdateView()
{
	if (m_modificatorData.m_moveObject && m_selectedObject)
	{
		m_mouseDx = m_lastMouseEvent.GetX();
		m_mouseDy = m_lastMouseEvent.GetY();
	}
	m_mouseX = m_lastMouseEvent.GetX();
	m_mouseY = m_lastMouseEvent.GetY();
	if (m_lastMouseEvent.ButtonIsDown(wxMouseButton::wxMOUSE_BTN_LEFT))
	{
		float x = (float)m_mouseDx - m_lastMouseEvent.GetX();
		float y = (float)m_mouseDy - m_lastMouseEvent.GetY();
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		if (c)
		{
			c->Rotate(x, y, 0);
		}
		m_mouseDx = m_lastMouseEvent.GetX();
		m_mouseDy = m_lastMouseEvent.GetY();
	}
	if (m_lastMouseEvent.ButtonIsDown(wxMouseButton::wxMOUSE_BTN_RIGHT))
	{
		float x = (float)m_mouseDx - m_lastMouseEvent.GetX();
		float y = (float)m_mouseDy - m_lastMouseEvent.GetY();
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		if (c)
		{
			c->Move(-x, y, 0, false);
		}
		m_mouseDx = m_lastMouseEvent.GetX();
		m_mouseDy = m_lastMouseEvent.GetY();
	}
}

void ZobEditorManager::UpdateGizmos()
{
	if (m_selectedObject)
	{
		ZobVector3 p = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera()->GetWorldPosition();
		p = m_selectedObject->GetWorldPosition() - p;
		float f = p.sqrtLength();
		ScaleGizmos(f / 8.0f);
		//Show(m_modificatorData.m_objectModificatorType);
		/*
		if (m_modificatorData.m_objectModificatorSpace == eGizmoModificatorSpace::space_local)
		{
			DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetEditorGizmos()->SetLocal();
		}
		else
		{
			DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetEditorGizmos()->SetWorld();
		}
		*/
	}
}

void ZobEditorManager::UpdateLog()
{
	std::vector<Events::event>* ev = DirectZob::GetInstance()->GetEventManager()->GetEventsStruct();
	for (std::vector<Events::event>::const_iterator iter = ev->begin(); iter != ev->end(); iter++)
	{
		MainWindowInterface::AddLog(*iter);
	}
	DirectZob::GetInstance()->GetEventManager()->ClearEvents();
	
}

void ZobEditorManager::OnNewScene()
{
	m_selectedObject = NULL;
	MainWindowInterface::OnNewScene();
	m_editorRootNode = new ZobObject(ZobEntity::type_editor, ZobEntity::subtype_zobOject, EDITOR_ROOT_NODE, NULL);
	std::string name = std::string("EditorCamera");
	Camera* c = DirectZob::GetInstance()->GetCameraManager()->CreateEditorCamera(name, Camera::eCamera_orbital_free, 45.0f, NULL);
	c->SetWorldPosition(-20, 20, -20);
	ZobVector3 v = ZobVector3(0, 0, 0);
	c->SetTarget(&v);

	m_gizmosNode = new ZobObject(ZobEntity::type_editor, ZobEntity::subtype_zobOject, EDITOR_GIZMOS_NODE, m_editorRootNode);

	ZobFilePath zfpT = ZobFilePath("arrow.obj", gArrow, gArrowLen);
	ZobFilePath zfpR = ZobFilePath("circle.obj", gCircle, gCircleLen);
	Triangle::RenderOptions::eLightMode lm = Triangle::RenderOptions::eLightMode_phong;

	m_translateX = new ZobObject(ZobEntity::type_editor, ZobEntity::subtype_zobOject, EDITOR_ARROW_X, m_gizmosNode);
	m_componentTranslateX = m_translateX->LoadMesh(zfpT, true);
	m_componentTranslateX->GetRenderOptions()->color = ZobColor::Red;
	m_componentTranslateX->GetRenderOptions()->lightMode = lm;
	m_componentTranslateX->GetRenderOptions()->zBuffer = Triangle::RenderOptions::halfBuffered;
	m_translateX->SetLocalRotation(0, 90, 0, false);

	m_translateY = new ZobObject(ZobEntity::type_editor, ZobEntity::subtype_zobOject, EDITOR_ARROW_Y, m_gizmosNode);
	m_componentTranslateY = m_translateY->LoadMesh(zfpT, true);
	m_componentTranslateY->GetRenderOptions()->color = ZobColor::Blue;
	m_componentTranslateY->GetRenderOptions()->lightMode = lm;
	m_componentTranslateY->GetRenderOptions()->zBuffer = Triangle::RenderOptions::halfBuffered;
	m_translateY->SetLocalRotation(-90, 0, 0, false);

	m_translateZ = new ZobObject(ZobEntity::type_editor, ZobEntity::subtype_zobOject, EDITOR_ARROW_Z, m_gizmosNode);
	m_componentTranslateZ = m_translateZ->LoadMesh(zfpT, true);
	m_componentTranslateZ->GetRenderOptions()->color = ZobColor::Green;
	m_componentTranslateZ->GetRenderOptions()->lightMode = lm;
	m_componentTranslateZ->GetRenderOptions()->zBuffer = Triangle::RenderOptions::halfBuffered;
	m_translateZ->SetLocalRotation(0, 0, 0, false);

	m_rotateX = new ZobObject(ZobEntity::type_editor, ZobEntity::subtype_zobOject, EDITOR_ROTATE_X, m_gizmosNode);
	m_componentRotateX = m_rotateX->LoadMesh(zfpR, true);
	m_componentRotateX->GetRenderOptions()->color = ZobColor::Red;
	m_componentRotateX->GetRenderOptions()->lightMode = lm;
	m_componentRotateX->GetRenderOptions()->zBuffer = Triangle::RenderOptions::halfBuffered;
	m_rotateX->SetLocalRotation(0, 0, 90, false);

	m_rotateY = new ZobObject(ZobEntity::type_editor, ZobEntity::subtype_zobOject, EDITOR_ROTATE_Y, m_gizmosNode);
	m_componentRotateY = m_rotateY->LoadMesh(zfpR, true);
	m_componentRotateY->GetRenderOptions()->color = ZobColor::Blue;
	m_componentRotateY->GetRenderOptions()->lightMode = lm;
	m_componentRotateY->GetRenderOptions()->zBuffer = Triangle::RenderOptions::halfBuffered;
	m_rotateY->SetLocalRotation(0, 0, 0, false);

	m_rotateZ = new ZobObject(ZobEntity::type_editor, ZobEntity::subtype_zobOject, EDITOR_ROTATE_Z, m_gizmosNode);
	m_componentRotateZ = m_rotateZ->LoadMesh(zfpR, true);
	m_componentRotateZ->GetRenderOptions()->color = ZobColor::Green;
	m_componentRotateZ->GetRenderOptions()->lightMode = lm;
	m_componentRotateZ->GetRenderOptions()->zBuffer = Triangle::RenderOptions::halfBuffered;
	m_rotateZ->SetLocalRotation(90, 0, 0, false);

	DirectZob::GetInstance()->GetEngine()->ShowGrid(true);
	DirectZob::GetInstance()->GetEngine()->SetRenderMode(eRenderMode::eRenderMode_fullframe);
	DirectZob::GetInstance()->GetEngine()->DrawGizmos(true);
	HideGizmos();
	MainWindowInterface::RefreshCamerasList();
	m_init = true;
}

void ZobEditorManager::SetSelectedObject(ZobObject* z)
{
	m_selectedObject = z;
	if (z)
	{
		ShowGizmos();
		m_gizmosNode->SetParent(z);
		m_gizmosNode->SetLocalPosition(0, 0, 0);
		m_gizmosNode->SetLocalRotation(0, 0, 0, false);
		ZobVariablesExposer* zvars = z->GetVariablesExposer();
		MainWindowInterface::SetCurrentZobVariables(zvars);
	}
	else
	{
		HideGizmos();
		m_gizmosNode->SetParent(m_editorRootNode);
	}
}

void ZobEditorManager::ScaleGizmos(float s)
{
	m_translateX->SetWorldScale(s, s, s);
	m_translateY->SetWorldScale(s, s, s);
	m_translateZ->SetWorldScale(s, s, s);
	m_rotateX->SetWorldScale(s, s, s);
	m_rotateY->SetWorldScale(s, s, s);
	m_rotateZ->SetWorldScale(s, s, s);
}

void ZobEditorManager::SetLastMouseEvent(wxMouseEvent& e)
{
	m_lastMouseEvent = e;
}

void ZobEditorManager::DrawGrid()
{
	if (!DirectZob::GetInstance()->GetEngine()->ShowGrid())
	{
		return;
	}
	Camera* cam = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	Engine* engine = DirectZob::GetInstance()->GetEngine();
	if (cam && engine)
	{
		ZobVector3 p = cam->GetWorldPosition();
		ZobVector3 f = cam->GetForward();
		float d = fabsf(p.y / f.y);
		p.x += d * f.x;
		p.y += d * f.y;
		p.z += d * f.z;
		f = cam->GetWorldPosition();
		d = (int)(f.sqrtLength() / 10.0f);
		int gridSize = 10 + 50 * d;
		int step = 1;
		if (d > 10)
		{
			step = 5;
		}
		if (d > 20)
		{
			step = 10;
		}
		ZobVector3 a;
		ZobVector3 b;
		bool bold;
		float cx = ((int)p.x / 5) * 5;
		float cz = ((int)p.z / 5) * 5;
		for (int i = -gridSize; i <= gridSize; i += step)
		{
			a.x = i + cx;
			b.x = i + cx;
			a.y = 0.0f;
			b.y = 0.0f;
			a.z = -gridSize + cz;
			b.z = gridSize + cz;
			bold = (int)a.x % 5 == 0;
			//bold = false;
			if (bold)
			{
				engine->QueueLine(cam, &a, &b, &ZobColor::Seal, bold, false);
			}
			else
			{
				engine->QueueLine(cam, &a, &b, &ZobColor::Trout, bold, false);
			}
		}
		for (int i = -gridSize; i <= gridSize; i += step)
		{
			a.z = i + cz;
			b.z = i + cz;
			a.y = 0.0f;
			b.y = 0.0f;
			a.x = -gridSize + cx;
			b.x = gridSize + cx;
			bold = (int)a.z % 5 == 0;
			//bold = false;
			if (bold)
			{
				engine->QueueLine(cam, &a, &b, &ZobColor::Seal, bold, false);
			}
			else
			{
				engine->QueueLine(cam, &a, &b, &ZobColor::Trout, bold, false);
			}
		}
		engine->QueueLine(cam, &ZobVector3::Vector3Zero, &ZobVector3::Vector3X, &ZobColor::Red, true, false);
		engine->QueueLine(cam, &ZobVector3::Vector3Zero, &ZobVector3::Vector3Y, &ZobColor::Green, true, false);
		engine->QueueLine(cam, &ZobVector3::Vector3Zero, &ZobVector3::Vector3Z, &ZobColor::Blue, true, false);
	}
}

void ZobEditorManager::UpdateGizmoSelection(ZobObject* z)
{
	m_componentTranslateX->GetRenderOptions()->color = ZobColor::Red;
	m_componentTranslateY->GetRenderOptions()->color = ZobColor::Green;
	m_componentTranslateZ->GetRenderOptions()->color = ZobColor::Blue;
	m_componentRotateX->GetRenderOptions()->color = ZobColor::Red;
	m_componentRotateY->GetRenderOptions()->color = ZobColor::Green;
	m_componentRotateZ->GetRenderOptions()->color = ZobColor::Blue;

	if (z == m_translateX)
	{
		m_currentSelectedGizmo = z;
		m_componentTranslateX->GetRenderOptions()->color = ZobColor::Yellow;
	}
	else if (z == m_translateY)
	{
		m_currentSelectedGizmo = z;
		m_componentTranslateY->GetRenderOptions()->color = ZobColor::Yellow;
	}
	else if (z == m_translateZ)
	{
		m_currentSelectedGizmo = z;
		m_componentTranslateZ->GetRenderOptions()->color = ZobColor::Yellow;
	}
	else if (z == m_rotateX)
	{
		m_currentSelectedGizmo = z;
		m_componentRotateX->GetRenderOptions()->color = ZobColor::Yellow;
	}
	else if (z == m_rotateY)
	{
		m_currentSelectedGizmo = z;
		m_componentRotateY->GetRenderOptions()->color = ZobColor::Yellow;
	}
	else if (z == m_rotateZ)
	{
		m_currentSelectedGizmo = z;
		m_componentRotateZ->GetRenderOptions()->color = ZobColor::Yellow;
	}
}



//-------------------------------------------------------------------------------------------------------------------------------------------------


long ZobEditorManager::gArrowLen = 7647;
char* ZobEditorManager::gArrow = 
"# Blender v2.92.0 OBJ File : ''\n\
# www.blender.org\n\
#mtllib arrow.mtl\n\
o Cylinder\n\
v 0.019752 0.099298 0.800040\n\
v 0.079080 0.064899 0.800123\n\
v 0.101818 -0.010028 0.800104\n\
v 0.071586 -0.071586 0.800050\n\
v 0.029164 -0.096142 0.800012\n\
v -0.019751 -0.099296 0.800044\n\
v -0.079084 -0.064902 0.800113\n\
v -0.101818 0.010028 0.800104\n\
v -0.071586 0.071586 0.800050\n\
v -0.029164 0.096142 0.800012\n\
v 0.000000 -0.000000 1.000000\n\
v 0.009250 0.028675 0.000000\n\
v 0.019085 0.023255 0.000000\n\
v 0.026319 0.014667 0.000000\n\
v 0.029928 0.003479 0.000000\n\
v 0.028981 -0.008239 0.000000\n\
v 0.021998 -0.020930 0.000000\n\
v 0.006663 -0.029624 0.000000\n\
v -0.008239 -0.028981 0.000000\n\
v -0.020930 -0.021998 0.000000\n\
v -0.028884 -0.008762 0.000000\n\
v -0.029928 0.003479 0.000000\n\
v -0.024820 0.017492 0.000000\n\
v -0.013735 0.026817 0.000000\n\
v -0.002427 0.030032 0.000000\n\
v 0.010919 -0.028333 0.800000\n\
v -0.003479 -0.029928 0.800000\n\
v -0.029919 0.005181 0.800000\n\
v -0.023622 0.018703 0.800000\n\
v -0.014667 0.026319 0.800000\n\
v -0.000755 0.030355 0.800000\n\
v 0.021213 0.021213 0.800000\n\
v 0.026817 0.013735 0.800000\n\
v 0.013735 0.026817 0.800000\n\
v -0.016824 -0.025179 0.800000\n\
v -0.027755 -0.012314 0.800000\n\
v 0.022952 -0.019520 0.800000\n\
v 0.028675 -0.009250 0.800000\n\
v 0.030032 0.002427 0.800000\n\
vt 0.296080 0.482406\n\
vt 0.250000 0.250000\n\
vt 0.431082 0.398610\n\
vt 0.483127 0.227039\n\
vt 0.417637 0.082567\n\
vt 0.319333 0.021440\n\
vt 0.203635 0.017650\n\
vt 0.068918 0.101390\n\
vt 0.016873 0.272961\n\
vt 0.082567 0.417637\n\
vt 0.180667 0.478560\n\
vt 0.987280 0.277617\n\
vt 0.801987 0.018620\n\
vt 0.521440 0.180667\n\
vt 0.818200 0.227987\n\
vt 0.821432 0.255761\n\
vt 0.983127 0.227039\n\
vt 1.000000 0.500000\n\
vt 0.990983 0.994555\n\
vt 0.950317 0.500000\n\
vt 0.924683 0.994555\n\
vt 0.890625 0.500000\n\
vt 0.830933 0.500000\n\
vt 0.875000 0.994555\n\
vt 0.825317 0.994555\n\
vt 0.768433 0.500000\n\
vt 0.762817 0.994555\n\
vt 0.705933 0.500000\n\
vt 0.700317 0.994555\n\
vt 0.628903 0.500000\n\
vt 0.637817 0.994555\n\
vt 0.558402 0.993616\n\
vt 0.535153 0.500000\n\
vt 0.481567 0.986903\n\
vt 0.455933 0.500000\n\
vt 0.406169 0.983138\n\
vt 0.296875 0.500000\n\
vt 0.222848 0.983138\n\
vt 0.231567 0.500000\n\
vt 0.152152 0.500000\n\
vt 0.080933 0.994555\n\
vt 0.075317 0.500000\n\
vt 0.018433 0.994555\n\
vt 0.012817 0.500000\n\
vt 0.958649 0.366318\n\
vt 0.901521 0.434629\n\
vt 0.979787 0.184712\n\
vt 0.921773 0.086497\n\
vt 0.556574 0.386627\n\
vt 0.641138 0.462635\n\
vt 0.730796 0.488109\n\
vt 0.823378 0.477333\n\
vt 0.684712 0.020213\n\
vt 0.586497 0.078227\n\
vt 0.512720 0.277618\n\
vt 0.680667 0.478560\n\
vt 0.796080 0.482406\n\
vt 0.748158 0.321020\n\
vt 0.931082 0.398610\n\
vt 0.782659 0.313790\n\
vt 0.813790 0.282659\n\
vt 0.800912 0.300912\n\
vt 0.715105 0.312595\n\
vt 0.582567 0.417637\n\
vt 0.516873 0.272961\n\
vt 0.693807 0.294476\n\
vt 0.568918 0.101390\n\
vt 0.685058 0.221200\n\
vt 0.703635 0.017650\n\
vt 0.710480 0.190920\n\
vt 0.775477 0.183681\n\
vt 0.819333 0.021440\n\
vt 0.741715 0.178816\n\
vt 0.679985 0.262049\n\
vt 0.917637 0.082567\n\
vt 0.804584 0.203563\n\
vt 0.378903 0.500000\n\
vt 0.316405 0.983138\n\
vt 0.143433 0.987821\n\
vn 0.4519 0.7804 0.4322\n\
vn 0.8651 0.2624 0.4275\n\
vn 0.8093 -0.3978 0.4322\n\
vn 0.4497 -0.7776 0.4394\n\
vn 0.0581 -0.8964 0.4394\n\
vn -0.4519 -0.7804 0.4322\n\
vn -0.8651 -0.2624 0.4275\n\
vn -0.8093 0.3978 0.4322\n\
vn -0.4497 0.7776 0.4394\n\
vn -0.0581 0.8964 0.4394\n\
vn 0.0000 0.0000 -1.0000\n\
vn 0.0014 -0.0002 -1.0000\n\
vn 0.1155 0.9933 -0.0006\n\
vn 0.4826 0.8758 -0.0007\n\
vn 0.8002 0.5997 0.0002\n\
vn 0.9619 0.2735 0.0002\n\
vn 0.9933 -0.1155 0.0002\n\
vn 0.8735 -0.4868 -0.0002\n\
vn 0.5909 -0.8067 0.0007\n\
vn 0.1101 -0.9939 0.0010\n\
vn -0.3353 -0.9421 0.0009\n\
vn -0.9964 -0.0850 0.0002\n\
vn -0.9395 0.3425 -0.0007\n\
vn -0.6437 0.7652 -0.0003\n\
vn -0.2735 0.9619 0.0002\n\
vn 0.0005 0.0004 -1.0000\n\
vn 0.0016 0.0004 -1.0000\n\
vn 0.0016 0.0007 -1.0000\n\
vn 0.0015 0.0004 -1.0000\n\
vn 0.0014 0.0010 -1.0000\n\
vn -0.0001 0.0002 -1.0000\n\
vn -0.0009 -0.0000 -1.0000\n\
vn -0.0013 -0.0002 -1.0000\n\
vn -0.0004 0.0005 -1.0000\n\
vn -0.0016 -0.0006 -1.0000\n\
vn -0.0015 -0.0005 -1.0000\n\
vn -0.0006 -0.0004 -1.0000\n\
vn 0.0001 -0.0007 -1.0000\n\
vn -0.0002 -0.0006 -1.0000\n\
vn -0.0012 -0.0010 -1.0000\n\
vn 0.0011 0.0014 -1.0000\n\
vn 0.0001 0.0005 -1.0000\n\
vn -0.0015 -0.0002 -1.0000\n\
vn -0.0014 0.0007 -1.0000\n\
vn 0.0013 0.0003 -1.0000\n\
vn 0.0009 0.0000 -1.0000\n\
vn 0.0004 -0.0006 -1.0000\n\
vn 0.0014 -0.0008 -1.0000\n\
vn 0.2372 0.9715 0.0009\n\
vn 0.5997 0.8002 0.0004\n\
vn 0.7648 0.6442 -0.0004\n\
vn 0.9517 0.3070 -0.0002\n\
vn 0.9968 -0.0805 -0.0002\n\
vn 0.8761 -0.4821 -0.0003\n\
vn 0.4932 -0.8699 -0.0012\n\
vn -0.0431 -0.9991 -0.0009\n\
vn -0.4821 -0.8761 -0.0010\n\
vn -0.7621 -0.6475 0.0013\n\
vn -0.8571 -0.5151 -0.0011\n\
vn -0.9924 -0.1227 0.0009\n\
vn -0.9065 0.4222 0.0007\n\
vn -0.6479 0.7618 -0.0002\n\
vn -0.2786 0.9604 0.0003\n\
g Cylinder_Cylinder_None\n\
usemtl None\n\
s off\n\
f 1/1/1 11/2/1 2/3/1\n\
f 2/3/2 11/2/2 3/4/2\n\
f 3/4/3 11/2/3 4/5/3\n\
f 4/5/4 11/2/4 5/6/4\n\
f 5/6/5 11/2/5 6/7/5\n\
f 6/7/6 11/2/6 7/8/6\n\
f 7/8/7 11/2/7 8/9/7\n\
f 8/9/8 11/2/8 9/10/8\n\
f 9/10/9 11/2/9 10/11/9\n\
f 10/11/10 11/2/10 1/1/10\n\
f 15/12/11 18/13/11 21/14/11\n\
f 38/15/12 39/16/12 3/17/12\n\
f 25/18/13 31/19/13 12/20/13\n\
f 12/20/14 34/21/14 13/22/14\n\
f 14/23/15 32/24/15 33/25/15\n\
f 15/26/16 33/25/16 39/27/16\n\
f 16/28/17 39/27/17 38/29/17\n\
f 17/30/18 38/29/18 37/31/18\n\
f 17/30/19 37/31/19 26/32/19\n\
f 18/33/20 26/32/20 27/34/20\n\
f 19/35/21 27/34/21 35/36/21\n\
f 21/37/22 28/38/22 22/39/22\n\
f 22/39/23 28/38/23 23/40/23\n\
f 23/40/24 30/41/24 24/42/24\n\
f 24/42/25 31/43/25 25/44/25\n\
f 14/45/11 15/12/11 13/46/11\n\
f 16/47/11 17/48/11 15/12/11\n\
f 23/49/11 24/50/11 25/51/11\n\
f 25/51/11 12/52/11 13/46/11\n\
f 18/13/11 19/53/11 21/14/11\n\
f 19/53/11 20/54/11 21/14/11\n\
f 21/14/11 22/55/11 25/51/11\n\
f 22/55/11 23/49/11 25/51/11\n\
f 25/51/11 13/46/11 15/12/11\n\
f 15/12/11 17/48/11 18/13/11\n\
f 25/51/11 15/12/11 21/14/11\n\
f 10/56/26 1/57/26 31/58/26\n\
f 1/57/27 2/59/27 34/60/27\n\
f 2/59/28 3/17/28 33/61/28\n\
f 3/17/29 39/16/29 33/61/29\n\
f 2/59/30 33/61/30 32/62/30\n\
f 10/56/31 31/58/31 30/63/31\n\
f 9/64/32 10/56/32 30/63/32\n\
f 8/65/33 9/64/33 29/66/33\n\
f 29/66/34 9/64/34 30/63/34\n\
f 7/67/35 8/65/35 36/68/35\n\
f 6/69/36 7/67/36 35/70/36\n\
f 26/71/37 5/72/37 6/69/37\n\
f 27/73/38 26/71/38 6/69/38\n\
f 6/69/39 35/70/39 27/73/39\n\
f 7/67/40 36/68/40 35/70/40\n\
f 2/59/41 32/62/41 34/60/41\n\
f 1/57/42 34/60/42 31/58/42\n\
f 28/74/43 36/68/43 8/65/43\n\
f 8/65/44 29/66/44 28/74/44\n\
f 3/17/45 4/75/45 37/76/45\n\
f 4/75/46 5/72/46 26/71/46\n\
f 37/76/47 4/75/47 26/71/47\n\
f 3/17/48 37/76/48 38/15/48\n\
f 12/20/49 31/19/49 34/21/49\n\
f 13/22/50 34/21/50 32/24/50\n\
f 14/23/51 13/22/51 32/24/51\n\
f 15/26/52 14/23/52 33/25/52\n\
f 16/28/53 15/26/53 39/27/53\n\
f 17/30/54 16/28/54 38/29/54\n\
f 18/33/55 17/30/55 26/32/55\n\
f 19/35/56 18/33/56 27/34/56\n\
f 20/77/57 19/35/57 35/36/57\n\
f 20/77/58 35/36/58 36/78/58\n\
f 21/37/59 20/77/59 36/78/59\n\
f 21/37/60 36/78/60 28/38/60\n\
f 23/40/61 28/38/61 29/79/61\n\
f 23/40/62 29/79/62 30/41/62\n\
f 24/42/63 30/41/63 31/43/63";

long ZobEditorManager::gCircleLen = 24607;
char* ZobEditorManager::gCircle =
"# Blender v2.92.0 OBJ File: ''\n\
# www.blender.org\n\
#mtllib circle.mtl\n\
o Torus.001\n\
v 1.049937 -0.007279 -0.076994\n\
v 1.035149 0.037250 -0.066122\n\
v 0.950183 -0.013288 -0.062941\n\
v 0.976032 -0.040388 0.002491\n\
v 1.035515 -0.036499 0.067871\n\
v 0.938312 0.046735 -0.431211\n\
v 0.941038 0.035803 -0.220696\n\
v 1.014223 0.014725 -0.267672\n\
v 0.901681 -0.013432 -0.306260\n\
v 0.995682 -0.050664 -0.154330\n\
v 0.994496 -0.037751 -0.290752\n\
v 0.944021 -0.010924 -0.465539\n\
v 0.898757 0.049954 -0.471696\n\
v 0.842019 0.035798 -0.474011\n\
v 0.785539 0.011118 -0.537184\n\
v 0.802596 -0.035902 -0.536278\n\
v 0.817659 -0.048960 -0.617920\n\
v 0.797998 0.029271 -0.676335\n\
v 0.692914 -0.001832 -0.795139\n\
v 0.507921 0.049257 -0.887372\n\
v 0.656636 0.029955 -0.695850\n\
v 0.641520 -0.037290 -0.722843\n\
v 0.527274 0.036022 -0.807890\n\
v 0.530336 -0.008896 -0.790578\n\
v 0.609452 -0.038635 -0.837267\n\
v 0.470759 -0.012143 -0.940737\n\
v 0.458993 0.025305 -0.939184\n\
v 0.376626 -0.040395 -0.900401\n\
v 0.385433 -0.050076 -0.936478\n\
v 0.269708 0.035746 -0.929134\n\
v 0.259399 -0.013279 -0.918901\n\
v 0.224672 0.038482 -1.010954\n\
v 0.231623 -0.030115 -1.020172\n\
v 0.072661 0.014673 -1.048706\n\
v -0.042053 0.048149 -0.992554\n\
v 0.062112 -0.036356 -0.963565\n\
v -0.101253 -0.049610 -1.013186\n\
v -0.137973 0.050245 -1.008376\n\
v -0.004011 0.012379 -0.949762\n\
v -0.080472 -0.025078 -1.042184\n\
v -0.201785 0.012223 -1.033053\n\
v -0.185775 0.013397 -0.933955\n\
v -0.189129 -0.036416 -0.946961\n\
v -0.371985 0.038635 -0.966475\n\
v -0.356649 -0.028526 -0.983260\n\
v -0.425902 0.036373 -0.866591\n\
v -0.420964 -0.013312 -0.854176\n\
v -0.548232 -0.038905 -0.881004\n\
v -0.589523 0.012143 -0.871243\n\
v -0.610147 0.049331 -0.816101\n\
v -0.484171 -0.049665 -0.864599\n\
v -0.661800 0.036587 -0.705944\n\
v -0.628062 -0.013148 -0.715786\n\
v -0.773917 -0.031278 -0.702032\n\
v -0.799376 0.030292 -0.674132\n\
v -0.738257 -0.049481 -0.668927\n\
v -0.833804 0.036448 -0.492919\n\
v -0.826395 -0.000664 -0.476634\n\
v -0.852558 -0.036783 -0.458295\n\
v -0.947075 0.027036 -0.444416\n\
v -0.955443 0.049874 -0.340678\n\
v -0.925462 -0.030115 -0.487771\n\
v -0.995145 -0.049925 -0.207646\n\
v -1.030524 -0.012143 -0.211238\n\
v -1.048804 0.015330 -0.106144\n\
v -0.968481 0.036981 -0.063260\n\
v -0.934072 0.013341 -0.185391\n\
v -0.946808 -0.036356 -0.189394\n\
v -1.014486 0.049725 -0.064317\n\
v -0.955072 -0.028378 0.064373\n\
v -1.024624 -0.048438 -0.009072\n\
v -1.034944 0.000000 0.205863\n\
v -1.019934 0.038990 0.177511\n\
v -0.934047 0.013256 0.185284\n\
v -0.952211 0.049972 0.276158\n\
v -0.914486 -0.036739 0.310705\n\
v -0.987433 -0.026197 0.348804\n\
v -0.925462 0.030115 0.487770\n\
v -0.877345 0.013390 0.365085\n\
v -0.911079 -0.049976 0.448418\n\
v -0.721101 0.048356 0.726542\n\
v -0.802715 0.035993 0.536357\n\
v -0.792042 -0.013256 0.528624\n\
v -0.756450 -0.050116 0.647053\n\
v -0.872770 -0.026162 0.574820\n\
v -0.792927 0.014673 0.690168\n\
v -0.710342 -0.038422 0.756838\n\
v -0.622600 0.049539 0.770634\n\
v -0.627811 0.023946 0.723110\n\
v -0.627890 -0.013198 0.715971\n\
v -0.530653 -0.036845 0.807343\n\
v -0.488588 -0.031808 0.922561\n\
v -0.600928 0.028043 0.858475\n\
v -0.421616 0.013256 0.853823\n\
v -0.341021 0.003095 0.997918\n\
v -0.290752 0.037751 0.994497\n\
v -0.306082 0.049876 0.944333\n\
v -0.245950 -0.036237 0.937146\n\
v -0.249691 0.012379 0.916361\n\
v -0.193820 -0.049564 1.002908\n\
v -0.066482 0.013444 1.049845\n\
v -0.062779 0.013256 0.950175\n\
v -0.075273 -0.026162 1.042343\n\
v -0.040944 0.052611 1.003474\n\
v 0.064190 -0.036356 0.963429\n\
v 0.205092 -0.013417 1.031069\n\
v 0.202917 0.034985 1.020138\n\
v 0.185531 0.013148 0.934018\n\
v 0.247595 -0.049797 0.988326\n\
v 0.400694 0.050329 0.907556\n\
v 0.305118 -0.037297 0.917008\n\
v 0.444250 -0.004059 0.956949\n\
v 0.560173 0.039426 0.873365\n\
v 0.307323 0.037874 0.917945\n\
v 0.421547 -0.013341 0.853908\n\
v 0.605151 0.036553 0.755513\n\
v 0.608845 -0.049322 0.782473\n\
v 0.678133 -0.041578 0.785846\n\
v 0.688550 0.000114 0.799288\n\
v 0.628235 -0.013256 0.715607\n\
v 0.841240 0.046936 0.592856\n\
v 0.873161 -0.012223 0.587796\n\
v 0.793854 0.025620 0.530437\n\
v 0.802596 -0.035902 0.536278\n\
v 0.938411 -0.049224 0.403250\n\
v 0.995379 -0.015773 0.337885\n\
v 1.020319 0.017862 0.264874\n\
v 0.988816 0.049465 0.254196\n\
v 0.921637 0.042979 0.322947\n\
v 0.901550 0.013256 0.306564\n\
v 0.914601 -0.036757 0.310466\n\
v 0.955239 0.028639 0.056905\n\
vt 0.511239 0.480103\n\
vt 0.541667 0.546676\n\
vt 0.510620 0.620811\n\
vt 0.524587 0.263602\n\
vt 0.545566 0.385471\n\
vt 0.577352 0.695781\n\
vt 0.536471 0.873572\n\
vt 0.567771 0.594271\n\
vt 0.602728 0.310909\n\
vt 0.573037 0.462434\n\
vt 0.551772 0.044255\n\
vt 0.593750 0.122123\n\
vt 0.611777 0.574526\n\
vt 0.629400 0.892784\n\
vt 0.595935 0.976833\n\
vt 0.581671 0.881035\n\
vt 0.634107 0.147441\n\
vt 0.635583 0.498214\n\
vt 0.655922 0.021502\n\
vt 0.625000 0.000000\n\
vt 0.650105 0.377089\n\
vt 0.677653 0.583333\n\
vt 0.667212 0.682514\n\
vt 0.658044 0.887336\n\
vt 0.686950 0.153872\n\
vt 0.676656 0.460522\n\
vt 0.705681 0.031970\n\
vt 0.688699 0.294936\n\
vt 0.715283 0.631144\n\
vt 0.715056 0.426188\n\
vt 0.740048 0.544163\n\
vt 0.756416 0.798289\n\
vt 0.704687 0.884398\n\
vt 0.739925 0.126014\n\
vt 0.771767 0.694432\n\
vt 0.750000 0.961701\n\
vt 0.705681 1.000000\n\
vt 0.750000 0.000000\n\
vt 0.781726 0.540001\n\
vt 0.781126 0.954007\n\
vt 0.781100 0.126292\n\
vt 0.765464 0.308453\n\
vt 0.762270 0.416667\n\
vt 0.805124 0.421593\n\
vt 0.821942 0.877052\n\
vt 0.823802 0.041423\n\
vt 0.781126 0.000000\n\
vt 0.831257 0.207439\n\
vt 0.844178 0.539479\n\
vt 0.808174 0.616997\n\
vt 0.852280 0.700787\n\
vt 0.839007 0.368404\n\
vt 0.888914 0.577330\n\
vt 0.883015 0.424583\n\
vt 0.923080 0.425562\n\
vt 0.966778 0.318671\n\
vt 0.882802 0.217035\n\
vt 0.968322 0.460522\n\
vt 0.929597 0.576745\n\
vt 0.983870 0.564238\n\
vt 0.989502 0.706827\n\
vt 0.945051 0.687794\n\
vt 0.985572 0.843918\n\
vt 0.969275 0.000000\n\
vt 1.000000 0.133763\n\
vt 0.969153 0.121003\n\
vt 1.000000 0.424828\n\
vt 0.027539 0.630534\n\
vt 0.000000 0.706827\n\
vt 0.000000 0.541871\n\
vt 0.045159 0.782688\n\
vt 0.007818 0.854120\n\
vt 0.031364 0.955753\n\
vt 0.000000 1.000000\n\
vt 0.031250 0.500000\n\
vt 0.002246 0.424828\n\
vt 0.054822 0.426125\n\
vt 0.076920 0.574438\n\
vt 0.062500 0.000000\n\
vt 0.052131 0.127386\n\
vt 0.031364 0.000000\n\
vt 0.093364 0.882900\n\
vt 0.062500 0.958129\n\
vt 0.094766 0.044505\n\
vt 0.072352 0.318579\n\
vt 0.092691 0.416667\n\
vt 0.125221 0.665170\n\
vt 0.141124 0.752743\n\
vt 0.130030 0.377720\n\
vt 0.114155 0.544175\n\
vt 0.153356 0.569619\n\
vt 0.135417 0.042025\n\
vt 0.158134 0.151598\n\
vt 0.112644 0.203231\n\
vt 0.177198 0.955753\n\
vt 0.135417 1.000000\n\
vt 0.136210 0.916667\n\
vt 0.173003 0.418205\n\
vt 0.197395 0.502181\n\
vt 0.199978 0.804726\n\
vt 0.208775 0.127065\n\
vt 0.219275 0.317701\n\
vt 0.208333 0.000000\n\
vt 0.177198 0.000000\n\
vt 0.240542 0.542102\n\
vt 0.204481 0.620151\n\
vt 0.244588 0.695769\n\
vt 0.238524 0.416667\n\
vt 0.239698 0.955753\n\
vt 0.260014 0.121003\n\
vt 0.281627 0.616735\n\
vt 0.281536 0.000000\n\
vt 0.239698 0.000000\n\
vt 0.290184 0.302651\n\
vt 0.282047 0.458214\n\
vt 0.301355 0.872086\n\
vt 0.281535 0.959957\n\
vt 0.300433 0.154545\n\
vt 0.319715 0.486608\n\
vt 0.339886 0.623076\n\
vt 0.316828 0.768397\n\
vt 0.321775 0.040278\n\
vt 0.354873 0.256812\n\
vt 0.363372 0.353101\n\
vt 0.364655 0.500288\n\
vt 0.364469 0.044247\n\
vt 0.357965 0.872348\n\
vt 0.406250 0.916347\n\
vt 0.364469 1.000000\n\
vt 0.406602 0.121658\n\
vt 0.435398 0.313248\n\
vt 0.406144 0.460634\n\
vt 0.459992 0.683247\n\
vt 0.401644 0.620311\n\
vt 0.446355 0.833333\n\
vt 0.447917 0.128619\n\
vt 0.447169 0.454087\n\
vt 0.490136 0.859878\n\
vt 0.448933 0.955495\n\
vt 0.489277 0.379857\n\
vt 0.459877 0.565554\n\
vt 0.479167 0.000000\n\
vt 0.509861 0.042711\n\
vt 0.499613 0.150984\n\
vt 0.509861 1.000000\n\
vt 0.551772 1.000000\n\
vt 0.595935 0.000000\n\
vt 0.655922 1.000000\n\
vt 0.823802 1.000000\n\
vt 0.869036 0.864687\n\
vt 0.865176 1.000000\n\
vt 0.865176 0.039519\n\
vt 0.915096 0.878963\n\
vt 0.916520 0.985914\n\
vt 0.916520 0.016098\n\
vt 0.921929 0.140243\n\
vt 0.969275 0.960837\n\
vt 1.000000 1.000000\n\
vt 0.010673 0.109740\n\
vt 0.094766 1.000000\n\
vt 0.208333 0.961700\n\
vt 0.321775 1.000000\n\
vt 0.406250 0.000000\n\
vt 0.448933 0.000000\n\
vn 0.9301 0.3418 -0.1348\n\
vn 0.6854 -0.7243 -0.0745\n\
vn 0.0286 0.9983 0.0515\n\
vn 0.2345 -0.9676 -0.0936\n\
vn 0.9286 -0.2165 -0.3014\n\
vn -0.8071 -0.4425 0.3909\n\
vn 0.5733 -0.7687 -0.2836\n\
vn 0.7963 0.3657 -0.4819\n\
vn 0.3374 0.9054 -0.2576\n\
vn -0.7632 0.1223 0.6345\n\
vn -0.1428 -0.9811 0.1306\n\
vn 0.7561 -0.2588 -0.6011\n\
vn -0.5467 -0.2032 0.8123\n\
vn 0.5538 -0.6208 -0.5549\n\
vn 0.4922 0.5141 -0.7025\n\
vn -0.1201 0.9741 0.1915\n\
vn -0.5455 -0.1868 0.8170\n\
vn 0.5323 -0.3020 -0.7908\n\
vn -0.2691 -0.7581 0.5941\n\
vn 0.3015 -0.7451 -0.5949\n\
vn 0.2198 0.7898 -0.5727\n\
vn 0.2740 -0.6914 -0.6685\n\
vn 0.2158 0.1515 -0.9646\n\
vn -0.1811 0.2750 0.9442\n\
vn -0.1957 -0.1895 0.9622\n\
vn 0.0319 0.9976 -0.0613\n\
vn -0.1130 0.7106 0.6945\n\
vn -0.0921 0.2220 0.9707\n\
vn -0.1470 -0.4491 0.8813\n\
vn -0.0508 0.6023 -0.7966\n\
vn 0.0575 0.7905 0.6097\n\
vn 0.0144 -0.9768 0.2136\n\
vn -0.0574 0.0574 -0.9967\n\
vn -0.1397 -0.7043 -0.6960\n\
vn 0.2841 0.3381 0.8972\n\
vn 0.3365 -0.2590 0.9053\n\
vn 0.1461 -0.9150 0.3761\n\
vn -0.4109 0.1316 -0.9021\n\
vn -0.0849 0.9753 -0.2038\n\
vn 0.1891 0.8631 0.4684\n\
vn -0.0974 -0.9630 -0.2513\n\
vn -0.5140 0.6096 -0.6034\n\
vn -0.5916 -0.3386 -0.7316\n\
vn -0.8143 -0.0759 -0.5755\n\
vn -0.2191 -0.9679 -0.1230\n\
vn -0.9330 -0.0883 -0.3488\n\
vn -0.6172 0.7757 -0.1315\n\
vn 0.2653 0.9625 0.0572\n\
vn 0.9672 -0.2510 0.0394\n\
vn -0.7213 -0.6851 -0.1019\n\
vn -0.7168 0.6972 0.0148\n\
vn 0.2677 0.9622 -0.0497\n\
vn 0.9864 -0.0793 -0.1442\n\
vn -0.9474 -0.3190 0.0264\n\
vn 0.8263 0.5599 -0.0610\n\
vn -0.7269 -0.6766 0.1176\n\
vn -0.8200 0.5078 0.2642\n\
vn -0.3410 0.9310 0.1305\n\
vn 0.8914 -0.3558 -0.2808\n\
vn 0.7364 0.5504 -0.3935\n\
vn 0.8704 -0.1295 -0.4751\n\
vn -0.6409 -0.6954 0.3252\n\
vn 0.0211 0.9970 -0.0739\n\
vn -0.7132 -0.3388 0.6136\n\
vn -0.5680 0.5595 0.6036\n\
vn 0.5148 -0.5192 -0.6822\n\
vn -0.6545 -0.0732 0.7525\n\
vn 0.5345 0.1584 -0.8302\n\
vn -0.5756 -0.2452 0.7801\n\
vn -0.4665 0.0685 0.8818\n\
vn 0.4027 0.5458 -0.7348\n\
vn 0.1396 -0.9431 -0.3018\n\
vn 0.3184 -0.3518 -0.8802\n\
vn -0.1864 0.3607 0.9139\n\
vn -0.0665 0.9650 0.2536\n\
vn -0.1955 -0.4648 0.8636\n\
vn 0.1310 0.7711 -0.6231\n\
vn 0.0176 -0.9771 -0.2120\n\
vn 0.0107 0.7610 0.6487\n\
vn -0.0598 -0.3975 -0.9156\n\
vn -0.0136 -0.9182 -0.3958\n\
vn 0.0483 -0.1962 0.9794\n\
vn -0.2009 0.4117 -0.8889\n\
vn -0.1883 -0.1177 -0.9750\n\
vn -0.0436 -0.9775 -0.2065\n\
vn 0.2806 0.2234 0.9335\n\
vn 0.1433 0.9153 0.3765\n\
vn -0.3275 -0.5076 -0.7969\n\
vn 0.2595 -0.5927 0.7625\n\
vn -0.1710 -0.9377 -0.3023\n\
vn 0.2715 -0.7933 0.5450\n\
vn 0.5317 0.1751 0.8286\n\
vn -0.3485 -0.7790 -0.5212\n\
vn 0.5068 -0.3766 0.7754\n\
vn -0.7522 0.1630 -0.6385\n\
vn -0.6018 -0.6169 -0.5072\n\
vn 0.1062 -0.9901 0.0920\n\
vn 0.6882 -0.3727 0.6225\n\
vn -0.2785 0.9536 -0.1142\n\
vn 0.4569 -0.8271 0.3274\n\
vn -0.0295 0.9974 0.0653\n\
vn -0.1799 -0.9800 -0.0855\n\
vn 0.7499 -0.5454 0.3745\n\
vn -0.7861 0.6039 -0.1318\n\
vn 0.6241 -0.7665 0.1516\n\
vn 0.6939 0.7164 0.0730\n\
vn -0.8927 -0.4123 0.1819\n\
vn 0.2119 -0.9770 0.0243\n\
vn 0.9949 -0.0467 0.0896\n\
vn 0.1545 0.9879 -0.0153\n\
vn -0.5566 -0.8285 0.0617\n\
vn -0.9984 -0.0238 0.0505\n\
vn 0.6875 -0.7221 -0.0772\n\
vn 0.6417 0.7520 -0.1507\n\
vn -0.8961 0.4063 0.1784\n\
vn -0.3846 0.8587 0.3388\n\
vn 0.9423 -0.2632 -0.2069\n\
vn 0.8868 0.2981 -0.3533\n\
vn -0.9256 0.1111 0.3618\n\
vn -0.2446 0.9649 0.0956\n\
vn -0.7137 0.5616 0.4186\n\
vn -0.8633 -0.3054 0.4017\n\
vn -0.2751 -0.9560 0.1021\n\
vn 0.7758 -0.2406 -0.5833\n\
vn 0.2122 0.9563 -0.2011\n\
vn -0.7601 -0.1569 0.6305\n\
vn -0.2280 -0.9167 0.3282\n\
vn 0.4747 0.6525 -0.5907\n\
vn -0.6287 0.2420 0.7390\n\
vn 0.5304 0.2008 -0.8236\n\
vn -0.4113 0.2855 0.8656\n\
vn 0.1215 -0.9794 -0.1615\n\
vn -0.0878 0.9789 0.1844\n\
vn 0.2995 0.1486 -0.9425\n\
vn -0.0706 -0.9244 0.3748\n\
vn 0.0258 0.9890 -0.1454\n\
vn 0.0724 -0.2186 0.9731\n\
vn 0.0489 -0.9476 -0.3158\n\
vn 0.0218 0.8036 -0.5947\n\
vn 0.0321 -0.7743 -0.6320\n\
vn 0.0590 -0.3789 -0.9236\n\
vn 0.5443 0.2326 0.8060\n\
vn -0.1580 0.7104 -0.6858\n\
vn -0.1876 -0.3892 -0.9019\n\
vn -0.3390 0.1545 -0.9280\n\
vn 0.0043 0.9944 0.1059\n\
vn 0.2838 0.2589 0.9233\n\
vn 0.2360 -0.6239 0.7450\n\
vn -0.1142 -0.9439 -0.3098\n\
vn -0.3535 0.7089 -0.6103\n\
vn -0.4529 -0.2002 -0.8688\n\
vn 0.1147 0.9793 0.1671\n\
vn 0.4161 -0.7043 0.5752\n\
vn -0.1769 -0.9623 -0.2065\n\
vn -0.6816 0.0490 -0.7301\n\
vn 0.7195 0.3838 0.5788\n\
vn 0.5762 -0.6893 0.4391\n\
vn -0.2572 0.9501 -0.1762\n\
vn -0.8382 0.0909 -0.5377\n\
vn 0.5187 0.7611 0.3893\n\
vn 0.9437 0.1004 0.3152\n\
vn -0.2911 -0.9354 -0.2008\n\
vn 0.8562 -0.1991 0.4766\n\
vn -0.8890 0.3808 -0.2542\n\
vn -0.7413 0.6404 -0.2008\n\
vn 0.1760 0.9829 0.0540\n\
vn 0.8803 -0.3445 0.3262\n\
vn 0.2447 -0.9687 0.0407\n\
vn 0.2393 -0.9672 0.0854\n\
vn -0.7010 -0.6776 -0.2223\n\
vn 0.8667 0.4751 0.1522\n\
vn 0.3727 -0.9231 -0.0946\n\
vn -0.9072 -0.4179 -0.0486\n\
vn -0.9161 0.3964 0.0602\n\
vn -0.2143 0.9760 0.0385\n\
vn 0.9863 -0.0238 -0.1633\n\
vn 0.7289 0.2020 -0.6541\n\
vn -0.4114 -0.9061 0.0991\n\
vn -0.9235 0.1782 0.3396\n\
vn 0.6050 0.7729 -0.1914\n\
vn -0.2256 0.9669 0.1193\n\
vn 0.6021 0.7374 -0.3063\n\
vn 0.8738 0.2544 -0.4143\n\
vn 0.7824 -0.4880 -0.3870\n\
vn 0.1250 -0.9873 -0.0980\n\
vn -0.8838 -0.1339 0.4484\n\
vn -0.5501 0.7244 0.4155\n\
vn -0.2797 -0.9352 0.2171\n\
vn -0.3820 -0.8802 0.2816\n\
vn -0.8319 0.0722 0.5502\n\
vn 0.5271 0.7228 -0.4470\n\
vn -0.1445 0.9523 0.2687\n\
vn 0.4393 -0.8118 -0.3847\n\
vn -0.0974 -0.9823 0.1600\n\
vn 0.3427 0.8109 -0.4743\n\
vn 0.5240 -0.5025 -0.6877\n\
vn 0.3533 0.7496 -0.5597\n\
vn 0.0346 -0.8231 0.5668\n\
vn -0.3442 0.5727 0.7440\n\
vn -0.1505 0.9494 0.2755\n\
vn 0.4142 -0.0589 -0.9083\n\
vn 0.2200 0.7739 -0.5939\n\
vn -0.2428 -0.6071 0.7567\n\
vn -0.0688 0.7805 0.6214\n\
vn 0.1229 0.7055 -0.6979\n\
vn 0.1666 -0.3768 -0.9112\n\
vn -0.1786 -0.1447 0.9732\n\
vn 0.0843 -0.0501 -0.9952\n\
vn 0.0892 0.2232 0.9707\n\
vn -0.0376 0.8112 -0.5836\n\
vn 0.8336 0.3658 0.4139\n\
vn 0.0546 0.9720 0.2285\n\
vn -0.1670 0.5988 -0.7833\n\
vn 0.2910 0.6218 0.7271\n\
vn -0.2166 0.5367 -0.8155\n\
vn -0.1588 0.9410 -0.2988\n\
vn -0.4785 0.0301 -0.8776\n\
vn -0.3916 -0.3444 -0.8533\n\
vn 0.1009 -0.9794 0.1749\n\
vn 0.0933 0.9884 0.1199\n\
vn -0.5377 0.3716 -0.7568\n\
vn -0.5150 0.3778 -0.7694\n\
vn 0.5133 0.6731 0.5324\n\
vn -0.7057 0.7067 0.0507\n\
vn 0.7192 0.3360 0.6082\n\
vn -0.1769 0.9647 -0.1952\n\
vn -0.7174 -0.1662 -0.6765\n\
vn -0.4972 -0.7522 -0.4323\n\
vn -0.8810 -0.2092 -0.4243\n\
vn 0.6308 0.7235 0.2803\n\
vn -0.6700 0.6502 -0.3582\n\
vn -0.9612 -0.1874 -0.2024\n\
vn -0.1712 0.9824 -0.0746\n\
vn -0.3048 -0.9511 -0.0496\n\
vn 0.9285 -0.3334 0.1636\n\
vn 0.9516 0.3013 0.0603\n\
vn -0.9369 -0.3362 -0.0954\n\
usemtl None\n\
s off\n\
f 1/1/1 8/2/1 2/3/1\n\
f 10/4/2 11/5/2 1/1/2\n\
f 13/6/3 7/7/3 6/8/3\n\
f 17/9/4 11/5/4 10/4/4\n\
f 11/5/5 12/10/5 8/2/5\n\
f 9/11/6 16/12/6 10/4/6\n\
f 17/9/7 12/10/7 11/5/7\n\
f 12/10/8 18/13/8 6/8/8\n\
f 18/13/9 13/6/9 6/8/9\n\
f 21/14/10 15/15/10 14/16/10\n\
f 22/17/11 17/9/11 16/12/11\n\
f 19/18/12 18/13/12 17/9/12\n\
f 24/19/13 22/17/13 21/20/13\n\
f 25/21/14 19/18/14 17/9/14\n\
f 27/22/15 20/23/15 19/18/15\n\
f 20/23/16 23/24/16 21/14/16\n\
f 28/25/17 22/17/17 24/19/17\n\
f 25/21/18 26/26/18 19/18/18\n\
f 24/19/19 31/27/19 28/25/19\n\
f 29/28/20 26/26/20 25/21/20\n\
f 32/29/21 20/23/21 27/22/21\n\
f 29/28/22 33/30/22 26/26/22\n\
f 34/31/23 32/29/23 33/30/23\n\
f 20/23/24 35/32/24 30/33/24\n\
f 36/34/25 28/25/25 31/27/25\n\
f 38/35/26 35/32/26 32/29/26\n\
f 35/32/27 39/36/27 30/33/27\n\
f 39/36/28 31/37/28 30/33/28\n\
f 39/38/29 36/34/29 31/27/29\n\
f 41/39/30 38/35/30 34/31/30\n\
f 35/32/31 42/40/31 39/36/31\n\
f 43/41/32 37/42/32 36/34/32\n\
f 40/43/33 41/39/33 34/31/33\n\
f 45/44/34 40/43/34 37/42/34\n\
f 46/45/35 42/40/35 35/32/35\n\
f 47/46/36 43/41/36 42/47/36\n\
f 51/48/37 37/42/37 43/41/37\n\
f 49/49/38 44/50/38 45/44/38\n\
f 50/51/39 38/35/39 44/50/39\n\
f 50/51/40 46/45/40 38/35/40\n\
f 51/48/41 48/52/41 37/42/41\n\
f 55/53/42 50/51/42 49/49/42\n\
f 54/54/43 49/49/43 48/52/43\n\
f 54/54/44 62/55/44 55/53/44\n\
f 63/56/45 62/55/45 56/57/45\n\
f 62/55/46 64/58/46 60/59/46\n\
f 65/60/47 69/61/47 61/62/47\n\
f 69/61/48 66/63/48 61/62/48\n\
f 67/64/49 70/65/49 68/66/49\n\
f 63/56/50 71/67/50 64/58/50\n\
f 73/68/51 69/69/51 65/70/51\n\
f 75/71/52 66/72/52 69/69/52\n\
f 74/73/53 70/74/53 66/72/53\n\
f 72/75/54 65/70/54 71/76/54\n\
f 75/71/55 74/73/55 66/72/55\n\
f 77/77/56 72/75/56 71/76/56\n\
f 78/78/57 73/68/57 72/75/57\n\
f 78/78/58 75/71/58 73/68/58\n\
f 79/79/59 76/80/59 74/81/59\n\
f 82/82/60 79/83/60 75/71/60\n\
f 83/84/61 76/80/61 79/79/61\n\
f 80/85/62 85/86/62 77/77/62\n\
f 81/87/63 88/88/63 82/82/63\n\
f 87/89/64 86/90/64 85/86/64\n\
f 93/91/65 81/87/65 86/90/65\n\
f 90/92/66 91/93/66 84/94/66\n\
f 87/89/67 93/91/67 86/90/67\n\
f 94/95/68 90/96/68 89/97/68\n\
f 92/98/69 93/91/69 87/89/69\n\
f 95/99/70 93/91/70 92/98/70\n\
f 97/100/71 94/95/71 88/88/71\n\
f 98/101/72 100/102/72 91/93/72\n\
f 99/103/73 98/101/73 94/104/73\n\
f 101/105/74 96/106/74 95/99/74\n\
f 104/107/75 97/100/75 96/106/75\n\
f 100/102/76 103/108/76 95/99/76\n\
f 104/107/77 102/109/77 97/100/77\n\
f 105/110/78 100/102/78 98/101/78\n\
f 107/111/79 104/107/79 101/105/79\n\
f 108/112/80 105/110/80 102/113/80\n\
f 105/110/81 109/114/81 100/102/81\n\
f 106/115/82 101/105/82 103/108/82\n\
f 114/116/83 108/117/83 104/107/83\n\
f 108/112/84 111/118/84 105/110/84\n\
f 111/118/85 109/114/85 105/110/85\n\
f 112/119/86 107/111/86 106/115/86\n\
f 113/120/87 110/121/87 107/111/87\n\
f 115/122/88 111/118/88 108/112/88\n\
f 109/114/89 112/119/89 106/115/89\n\
f 117/123/90 109/114/90 111/118/90\n\
f 118/124/91 112/119/91 109/114/91\n\
f 119/125/92 113/120/92 112/119/92\n\
f 120/126/93 117/123/93 115/122/93\n"
"f 118/124/94 119/125/94 112/119/94\n\
f 116/127/95 123/128/95 120/129/95\n\
f 124/130/96 117/123/96 120/126/96\n\
f 125/131/97 118/124/97 117/123/97\n\
f 118/124/98 122/132/98 119/125/98\n\
f 128/133/99 123/128/99 121/134/99\n\
f 125/131/100 122/132/100 118/124/100\n\
f 128/133/101 129/135/101 123/128/101\n\
f 131/136/102 125/131/102 124/130/102\n\
f 125/131/103 126/137/103 122/132/103\n\
f 132/138/104 130/139/104 129/135/104\n\
f 5/140/105 126/137/105 125/131/105\n\
f 2/3/106 128/133/106 127/141/106\n\
f 132/142/107 3/143/107 4/144/107\n\
f 10/4/108 5/140/108 125/131/108\n\
f 5/140/109 1/1/109 127/141/109\n\
f 2/3/110 6/8/110 128/133/110\n\
f 4/144/111 3/143/111 9/11/111 10/4/111\n\
f 132/138/112 7/7/112 3/145/112\n\
f 10/4/113 1/1/113 5/140/113\n\
f 8/2/114 6/8/114 2/3/114\n\
f 7/7/115 9/146/115 3/145/115\n\
f 14/16/116 13/6/116 20/23/116 21/14/116\n\
f 11/5/117 8/2/117 1/1/117\n\
f 12/10/118 6/8/118 8/2/118\n\
f 7/7/119 14/16/119 9/146/119\n\
f 13/6/120 14/16/120 7/7/120\n\
f 14/16/121 15/15/121 9/146/121\n\
f 15/147/122 16/12/122 9/11/122\n\
f 10/4/123 16/12/123 17/9/123\n\
f 17/9/124 18/13/124 12/10/124\n\
f 18/13/125 20/23/125 13/6/125\n\
f 22/17/126 16/12/126 15/147/126 21/20/126\n\
f 17/9/127 22/17/127 28/25/127 29/28/127\n\
f 18/13/128 19/18/128 20/23/128\n\
f 23/24/129 24/148/129 21/14/129\n\
f 26/26/130 27/22/130 19/18/130\n\
f 24/148/131 23/24/131 30/33/131 31/37/131\n\
f 17/9/132 29/28/132 25/21/132\n\
f 20/23/133 30/33/133 23/24/133\n\
f 27/22/134 26/26/134 33/30/134 32/29/134\n\
f 29/28/135 28/25/135 36/34/135 37/42/135\n\
f 32/29/136 35/32/136 20/23/136\n\
f 43/41/137 36/34/137 39/38/137 42/47/137\n\
f 29/28/138 37/42/138 33/30/138\n\
f 34/31/139 38/35/139 32/29/139\n\
f 37/42/140 40/43/140 33/30/140\n\
f 33/30/141 40/43/141 34/31/141\n\
f 47/149/142 46/45/142 52/150/142 53/151/142\n\
f 41/39/143 44/50/143 38/35/143\n\
f 40/43/144 45/44/144 41/39/144\n\
f 41/39/145 45/44/145 44/50/145\n\
f 38/35/146 46/45/146 35/32/146\n\
f 46/45/147 47/149/147 42/40/147\n\
f 43/41/148 47/46/148 51/48/148\n\
f 37/42/149 48/52/149 45/44/149\n\
f 44/50/150 49/49/150 50/51/150\n\
f 48/52/151 49/49/151 45/44/151\n\
f 50/51/152 52/150/152 46/45/152\n\
f 56/57/153 51/48/153 47/46/153 53/152/153\n\
f 48/52/154 51/48/154 56/57/154 54/54/154\n\
f 49/49/155 54/54/155 55/53/155\n\
f 53/151/156 52/150/156 57/153/156 58/154/156\n\
f 56/57/157 53/152/157 58/155/157 59/156/157\n\
f 61/62/158 50/51/158 55/53/158 60/59/158\n\
f 62/55/159 60/59/159 55/53/159\n\
f 57/153/160 52/150/160 50/51/160 61/62/160\n\
f 67/157/161 58/154/161 57/153/161 66/63/161\n\
f 56/57/162 62/55/162 54/54/162\n\
f 59/156/163 63/56/163 56/57/163\n\
f 64/58/164 65/60/164 60/59/164\n\
f 60/59/165 65/60/165 61/62/165\n\
f 61/62/166 66/63/166 57/153/166\n\
f 59/156/167 58/155/167 67/64/167 68/66/167\n\
f 71/67/168 63/56/168 68/66/168 70/65/168\n\
f 59/156/169 68/66/169 63/56/169\n\
f 63/56/170 64/58/170 62/55/170\n\
f 66/63/171 70/158/171 67/157/171\n\
f 71/76/172 70/159/172 76/80/172 80/85/172\n\
f 71/67/173 65/60/173 64/58/173\n\
f 72/75/174 73/68/174 65/70/174\n\
f 73/68/175 75/71/175 69/69/175\n\
f 74/81/176 76/80/176 70/159/176\n\
f 90/96/177 83/160/177 82/82/177 89/97/177\n\
f 80/85/178 77/77/178 71/76/178\n\
f 72/75/179 77/77/179 78/78/179\n\
f 75/71/180 79/83/180 74/73/180\n\
f 78/78/181 81/87/181 75/71/181\n\
f 81/87/182 82/82/182 75/71/182\n\
f 82/82/183 83/160/183 79/83/183\n\
f 76/80/184 83/84/184 84/94/184\n\
f 76/80/185 84/94/185 80/85/185\n\
f 85/86/186 78/78/186 77/77/186\n\
f 86/90/187 81/87/187 78/78/187\n\
f 84/94/188 87/89/188 80/85/188\n\
f 80/85/189 87/89/189 85/86/189\n\
f 85/86/190 86/90/190 78/78/190\n\
f 88/88/191 89/97/191 82/82/191\n\
f 88/88/192 81/87/192 93/91/192 97/100/192\n\
f 83/84/193 90/92/193 84/94/193\n\
f 92/98/194 87/89/194 84/94/194 100/102/194\n\
f 88/88/195 94/95/195 89/97/195\n\
f 90/92/196 94/104/196 91/93/196\n\
f 91/93/197 100/102/197 84/94/197\n\
f 103/108/198 100/102/198 109/114/198 106/115/198\n\
f 95/99/199 96/106/199 93/91/199\n\
f 93/91/200 96/106/200 97/100/200\n\
f 94/104/201 98/101/201 91/93/201\n\
f 97/100/202 99/161/202 94/95/202\n\
f 100/102/203 95/99/203 92/98/203\n\
f 101/105/204 104/107/204 96/106/204\n\
f 97/100/205 102/109/205 99/161/205\n\
f 99/103/206 102/113/206 98/101/206\n\
f 103/108/207 101/105/207 95/99/207\n\
f 102/113/208 105/110/208 98/101/208\n\
f 101/105/209 106/115/209 107/111/209\n\
f 104/107/210 108/117/210 102/109/210\n\
f 127/141/211 121/134/211 122/132/211 126/137/211\n\
f 107/111/212 110/121/212 104/107/212\n\
f 104/107/213 110/121/213 114/116/213\n\
f 112/119/214 113/120/214 107/111/214\n\
f 108/117/215 114/116/215 115/162/215\n\
f 110/121/216 116/127/216 114/116/216\n\
f 114/116/217 116/127/217 115/162/217\n\
f 115/122/218 117/123/218 111/118/218\n\
f 109/114/219 117/123/219 118/124/219\n\
f 113/120/220 121/134/220 110/121/220\n\
f 110/121/221 121/134/221 116/127/221\n\
f 116/127/222 120/129/222 115/162/222\n\
f 119/125/223 121/134/223 113/120/223\n\
f 132/138/224 128/133/224 6/8/224 7/7/224\n\
f 122/132/225 121/134/225 119/125/225\n\
f 121/134/226 123/128/226 116/127/226\n\
f 123/163/227 124/130/227 120/126/227\n\
f 117/123/228 124/130/228 125/131/228\n\
f 131/136/229 124/130/229 123/163/229 130/164/229\n\
f 127/141/230 128/133/230 121/134/230\n\
f 129/135/231 130/139/231 123/128/231\n\
f 131/136/232 130/164/232 132/142/232 4/144/232\n\
f 128/133/233 132/138/233 129/135/233\n\
f 131/136/234 4/144/234 125/131/234\n\
f 126/137/235 5/140/235 127/141/235\n\
f 1/1/236 2/3/236 127/141/236\n\
f 4/144/237 10/4/237 125/131/237";