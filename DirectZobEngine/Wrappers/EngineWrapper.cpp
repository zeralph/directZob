#ifdef _WINDLL
#include "EngineWrapper.h"
#include "DirectZobWrapper.h"
#include "ZobObjectManagerWrapper.h"
#include "../ZobObjects/ZobObject.h"
#include "../Misc/ZobGeometryHelper.h"
namespace CLI
{
	static Line m_lines[NB_EDITOR_LINES];
	static Circle m_circles[NB_EDITOR_CIRCLES];

	EngineWrapper::EngineWrapper(PictureBox^ renderWindow):ManagedObject(DirectZob::GetInstance()->GetEngine(), false)
	{
		m_renderWindow = renderWindow;
		m_mouseInside = true;
		m_running = true;
		m_renderWindowGraphics = m_renderWindow->CreateGraphics();
		m_nbTriangles = 0;
		m_triangleList = (Triangle*)malloc(sizeof(Triangle) * NB_EDITOR_TRIANGLES);
		m_vertices = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_EDITOR_TRIANGLES * 3);
		m_uvs = (ZobVector2*)malloc(sizeof(ZobVector2) * NB_EDITOR_TRIANGLES * 3);
		m_projectedVertices = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_EDITOR_TRIANGLES * 3);
		m_normals = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_EDITOR_TRIANGLES * 3);
		m_modificatorData = DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetEditorGizmos()->GetModificatorData();
		m_modificatorData->Reset();
		m_modificatorData->m_objectModificatorType = ZobObjectsEditor::eGizmoModificatorType::eGizmo_translate;
		m_modificatorData->m_objectModificatorSpace = ZobObjectsEditor::eGizmoModificatorSpace::space_local;
		m_lastMouseX = 0;
		m_lastMouseY = 0;
		int vi = 0;
		for (int i = 0; i < NB_EDITOR_TRIANGLES; i++)
		{
			m_triangleList[i].verticeAIndex = vi;
			m_triangleList[i].verticeBIndex = vi + 1;
			m_triangleList[i].verticeCIndex = vi + 2;
			m_triangleList[i].va = &m_vertices[vi];
			m_triangleList[i].vb = &m_vertices[vi + 1];
			m_triangleList[i].vc = &m_vertices[vi + 2];
			m_triangleList[i].pa = &m_projectedVertices[vi];
			m_triangleList[i].pb = &m_projectedVertices[vi + 1];
			m_triangleList[i].pc = &m_projectedVertices[vi + 2];
			m_triangleList[i].na = &m_normals[vi];
			m_triangleList[i].nb = &m_normals[vi + 1];
			m_triangleList[i].nc = &m_normals[vi + 2];
			m_triangleList[i].ua = &m_uvs[vi];
			m_triangleList[i].ub = &m_uvs[vi + 1];
			m_triangleList[i].uc = &m_uvs[vi + 2];
			m_triangleList[i].n = &m_normals[i];
			m_triangleList[i].options = NULL;
			vi += 3;
		}
		ZobObjectManagerWrapper::OnObjectSelectedEvent += gcnew ZobObjectManagerWrapper::OnObjectSelected(this, &EngineWrapper::OnObjectSelected);
		DirectZobWrapperEvents::OnNewSceneEvent += gcnew DirectZobWrapperEvents::OnNewScene(this, &EngineWrapper::OnNewScene);
		
		m_renderWindow->MouseEnter += gcnew EventHandler(this, &EngineWrapper::OnMouseHover);
		m_renderWindow->MouseLeave += gcnew EventHandler(this, &EngineWrapper::OnMouseLeave);
		m_renderWindow->MouseWheel += gcnew MouseEventHandler(this, &EngineWrapper::OnMouseWheel);
		m_renderWindow->MouseDoubleClick+= gcnew MouseEventHandler(this, &EngineWrapper::OnMouseClick);
		m_renderWindow->MouseDown+= gcnew MouseEventHandler(this, &EngineWrapper::OnMouseDown);
		m_renderWindow->MouseUp += gcnew MouseEventHandler(this, &EngineWrapper::OnMouseUp);
		m_renderWindow->MouseMove += gcnew MouseEventHandler(this, &EngineWrapper::OnMouseMove);
		m_renderWindow->AutoSize = false;
		m_renderWindow->Width = 800;
		m_renderWindow->Height = 600;
	}

	EngineWrapper::~EngineWrapper()
	{
		free(m_triangleList);
		free(m_vertices);
		free(m_uvs);
		free(m_projectedVertices);
		free(m_normals);
	}

	int EngineWrapper::GetBufferWidth()
	{
		return m_Instance->GetBufferData()->width;
	}
	int EngineWrapper::GetBufferHeight()
	{
		return m_Instance->GetBufferData()->height;
	}
	void EngineWrapper::SetLightingPrecision(int r)
	{
		if (r >= 0 && r < (int)eLightingPrecision::__eLightingPrecision_MAX__)
		{
			m_Instance->SetLightingPrecision((eLightingPrecision)r);
		}
	}
	void EngineWrapper::SetRenderMode(int r)
	{
		if (r >= 0 && r < (int)eRenderMode::__eRenderMode_MAX__)
		{
			m_Instance->SetRenderMode((eRenderMode)r);
		}
	}

	void EngineWrapper::OnMouseHover(Object^ sender, EventArgs^ e)
	{
		m_mouseInside = true;
	}

	void EngineWrapper::OnMouseLeave(Object^ sender, EventArgs^ e)
	{
		m_mouseInside = false;
	}

	void EngineWrapper::Resize(int w, int h)
	{
		DirectZob::GetInstance()->GetEngine()->Resize(w, h);
	}

	void EngineWrapper::OnMouseDown(Object^ sender, MouseEventArgs^ e)
	{
		if (e && e->Button == MouseButtons::Left)
		{
			if (!m_modificatorData->m_moveObject && DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetSelectedObject())
			{
				m_modificatorData->m_moveObject = true;
				Point location = m_renderWindow->PointToClient(m_mouseCoords);
				ZobObject* z = DirectZob::GetInstance()->GetEngine()->GetObjectAt2DCoords(location.X, location.Y, true);
				if (z)
				{
					if (z->IsEditorObject())
					{
						ZobObject* pz = z->GetParent();
						m_modificatorData->m_currentObjectModificator = z;
						if (pz && pz->GetName() == EDITOR_OBJECT)
						{
							std:string s = z->GetName();
							if (s == EDITOR_ARROW_X || s == EDITOR_ROTATE_X)
							{
								m_modificatorData->m_objectModificatorAxis = ZobObjectsEditor::eGizmoModificatorAxis::axis_x;
							}
							else if (s == EDITOR_ARROW_Y || s == EDITOR_ROTATE_Y)
							{
								m_modificatorData->m_objectModificatorAxis = ZobObjectsEditor::eGizmoModificatorAxis::axis_y;
							}
							else if (s == EDITOR_ARROW_Z || s == EDITOR_ROTATE_Z)
							{
								m_modificatorData->m_objectModificatorAxis = ZobObjectsEditor::eGizmoModificatorAxis::axis_z;
							}
							SetupObjectModificator(DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetSelectedObject());
						}
					}
				}
			}
		}
	}

	void EngineWrapper::OnMouseMove(Object^ sender, MouseEventArgs^ e)
	{
		Point p = System::Windows::Forms::Control::MousePosition;
		p = m_renderWindow->PointToClient(p);
		m_lastMouseX = p.X;
		m_lastMouseY = p.Y;
	}

	void EngineWrapper::SetupObjectModificator(ZobObject* curObj)
	{
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		if (curObj && m_modificatorData->m_currentObjectModificator && m_modificatorData->m_moveObject)
		{
			m_modificatorData->m_currentModifiedObject = curObj;
			m_modificatorData->m_objectPosition = curObj->GetWorldPosition();
			if (m_modificatorData->m_objectModificatorType == ZobObjectsEditor::eGizmoModificatorType::eGizmo_translate)
			{
				if (m_modificatorData->m_objectModificatorAxis == ZobObjectsEditor::eGizmoModificatorAxis::axis_x)
				{
					if (m_modificatorData->m_objectModificatorSpace == ZobObjectsEditor::eGizmoModificatorSpace::space_local)
					{
						m_modificatorData->m_planeNormal = curObj->GetUp();
						m_modificatorData->m_objectDirection = curObj->GetLeft();
					}
					else
					{
						m_modificatorData->m_planeNormal = ZobVector3::Vector3Y;
						m_modificatorData->m_objectDirection = ZobVector3::Vector3X;
					}
				}
				else if (m_modificatorData->m_objectModificatorAxis == ZobObjectsEditor::eGizmoModificatorAxis::axis_y)
				{
					if (m_modificatorData->m_objectModificatorSpace == ZobObjectsEditor::eGizmoModificatorSpace::space_local)
					{
						m_modificatorData->m_planeNormal = curObj->GetLeft();
						m_modificatorData->m_objectDirection = curObj->GetUp();
					}
					else
					{
						m_modificatorData->m_planeNormal = ZobVector3::Vector3Z;
						m_modificatorData->m_objectDirection = ZobVector3::Vector3Y;
					}
				}
				else if (m_modificatorData->m_objectModificatorAxis == ZobObjectsEditor::eGizmoModificatorAxis::axis_z)
				{
					if (m_modificatorData->m_objectModificatorSpace == ZobObjectsEditor::eGizmoModificatorSpace::space_local)
					{
						m_modificatorData->m_planeNormal = curObj->GetLeft();
						m_modificatorData->m_objectDirection = curObj->GetForward();
					}
					else
					{
						m_modificatorData->m_planeNormal = ZobVector3::Vector3X;
						m_modificatorData->m_objectDirection = ZobVector3::Vector3Z;
					}
				}
				BufferData* bData = m_Instance->GetBufferData();
				float fx = (float)m_lastMouseX;
				float fy = (float)m_lastMouseY;
				fx /= (float)bData->width;
				fy /= (float)bData->height;
				fx = fx * 2.0f - 1.0f;
				fy = fy * 2.0f - 1.0f;
				ZobVector3 ret = c->From2DToWorldOnPlane(fx, fy, &m_modificatorData->m_planePosition, &m_modificatorData->m_planeNormal);
				ret = ZobGeometryHelper::ProjectPointOnLine(&m_modificatorData->m_objectPosition, &m_modificatorData->m_objectDirection, &ret);
				ret = ret - m_modificatorData->m_objectPosition;
				//float d = ret.sqrtLength();
				m_modificatorData->m_deltaStart = ret;// m_modificatorData->m_objectDirection* d;
			}
			else if (m_modificatorData->m_objectModificatorType == ZobObjectsEditor::eGizmoModificatorType::eGizmo_rotate)
			{
				if (m_modificatorData->m_objectModificatorAxis == ZobObjectsEditor::eGizmoModificatorAxis::axis_x)
				{
					if (m_modificatorData->m_objectModificatorSpace == ZobObjectsEditor::eGizmoModificatorSpace::space_local)
					{
						m_modificatorData->m_planeNormal = curObj->GetLeft();
					}
					else
					{
						m_modificatorData->m_planeNormal = ZobVector3::Vector3X;
					}
				}
				else if (m_modificatorData->m_objectModificatorAxis == ZobObjectsEditor::eGizmoModificatorAxis::axis_y)
				{
					if (m_modificatorData->m_objectModificatorSpace == ZobObjectsEditor::eGizmoModificatorSpace::space_world)
					{
						m_modificatorData->m_planeNormal = curObj->GetUp();
					}
					else
					{
						m_modificatorData->m_planeNormal = ZobVector3::Vector3Y;
					}
				}
				else if (m_modificatorData->m_objectModificatorAxis == ZobObjectsEditor::eGizmoModificatorAxis::axis_z)
				{
					if (m_modificatorData->m_objectModificatorSpace == ZobObjectsEditor::eGizmoModificatorSpace::space_local)
					{
						m_modificatorData->m_planeNormal = curObj->GetForward();
					}
					else
					{
						m_modificatorData->m_planeNormal = ZobVector3::Vector3Z;
					}
				}
			}
		}
	}

	void EngineWrapper::UpdateMoveObject()
	{
		ZobObject* curObj = DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetSelectedObject();
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		
		if (curObj && m_modificatorData->m_currentObjectModificator && m_modificatorData->m_moveObject)
		{
			if (m_modificatorData->m_objectModificatorType == ZobObjectsEditor::eGizmoModificatorType::eGizmo_translate)
			{
				BufferData* bData = m_Instance->GetBufferData();
				float fx = (float)m_lastMouseX;
				float fy = (float)m_lastMouseY;
				fx /= (float)bData->width;
				fy /= (float)bData->height;
				fx = fx * 2.0f - 1.0f;
				fy = fy * 2.0f - 1.0f;
				ZobVector3 ret = c->From2DToWorldOnPlane(fx, fy, &m_modificatorData->m_planePosition, &m_modificatorData->m_planeNormal);
				ret = ZobGeometryHelper::ProjectPointOnLine(&m_modificatorData->m_objectPosition, &m_modificatorData->m_objectDirection, &ret);
				ret = ret - m_modificatorData->m_deltaStart;
				curObj->SetWorldPosition(ret.x, ret.y, ret.z);
				m_Instance->QueueLine(c, &m_modificatorData->m_objectPosition, &ret, 0xFFFF00, false, true);
				ZobMatrix4x4 m;
				m.Identity();
				m.AddTranslation(m_modificatorData->m_objectPosition);
				m.AddTranslation(m_modificatorData->m_deltaStart);
				m_Instance->QueueSphere(c, &m, 0.5f, 0xFFFFFF, false, true);

			}
			else if (m_modificatorData->m_objectModificatorType == ZobObjectsEditor::eGizmoModificatorType::eGizmo_rotate)
			{
				BufferData* bData = m_Instance->GetBufferData();
				float fx = (float)m_lastMouseX;
				float fy = (float)m_lastMouseY;
				fx /= (float)bData->width;
				fy /= (float)bData->height;
				fx = fx * 2.0f - 1.0f;
				fy = fy * 2.0f - 1.0f;
				/*
				ZobVector3 ret = c->From2DToWorldOnPlane(fx, fy, &planePos, &planeNormal);
				ZobVector3 s = ZobVector3(m_modificatorData->m_currentModifiedStartX, m_modificatorData->m_currentModifiedStartY, m_modificatorData->m_currentModifiedStartZ);
				ret = ZobGeometryHelper::ProjectPointOnLine(&s, &direction, &ret);
				curObj->SetWorldPosition(ret.x, ret.y, ret.z);
				*/
			}
		}
	}

	void EngineWrapper::OnMouseUp(Object^ sender, MouseEventArgs^ e)
	{
		m_modificatorData->m_moveObject = false;
		//m_modificatorData->Reset();
	}

	void EngineWrapper::OnMouseClick(Object^ sender, MouseEventArgs^ e)
	{
		Point location = m_renderWindow->PointToClient(m_mouseCoords);
		ZobObject* z = DirectZob::GetInstance()->GetEngine()->GetObjectAt2DCoords(location.X, location.Y, false);
		if (z)
		{
			if (z->IsEditorObject())
			{
				//DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->SelectObject(z);
			}
			else
			{
				DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->SelectObject(z);
			}
		}
		else
		{
			DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->SelectObject(NULL);
		}
	}

	void EngineWrapper::OnMouseWheel(Object^ sender, MouseEventArgs^ e)
	{
		if (e->Delta)
		{
			float z = (float)e->Delta / 40.0f;
			Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
			c->Zoom(z);
		}
		//m_mouseCoords = Cursor::Position;
	}

	void EngineWrapper::UpdateCameraEditor(float dt)
	{
		if (!m_mouseInside)
		{
			return;
		}
		ZobObject* curObj = DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetSelectedObject();
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && GetKeyState(VK_CONTROL) & 0x8000)
		{
			float factor = 20.0f;
			float x = Cursor::Position.X - m_mouseCoords.X;
			float y = Cursor::Position.Y - m_mouseCoords.Y;
			x *= dt* factor;
			y *= dt* factor;
			Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
			c->Rotate(-x, -y, 0);
			m_modificatorData->Reset();
			//c->Rotate(30, 0, 0);
		}
		else if (curObj == NULL && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			float factor = 40.0f;
			Point p;
			p.X = Cursor::Position.X - m_mouseCoords.X;
			p.Y = Cursor::Position.Y - m_mouseCoords.Y;
			p.X *= dt * factor;
			p.Y *= dt * factor;
			Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
			c->Move(p.X, -p.Y, 0, false);
			m_modificatorData->Reset();
		}
		m_mouseLastCoords = m_mouseCoords;
		m_mouseCoords = Cursor::Position;
	}

	void EngineWrapper::Update(float dt)
	{
		ZobVector3 p = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera()->GetWorldPosition();
		p = m_modificatorData->m_objectPosition - p;
		float f = p.sqrtLength();
		DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetEditorGizmos()->Scale(f / 4.0f);
		ZobObject* curObj = DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetSelectedObject();
		if (curObj)
		{
			DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetEditorGizmos()->Show(m_modificatorData->m_objectModificatorType);
			if (m_modificatorData->m_objectModificatorSpace == ZobObjectsEditor::eGizmoModificatorSpace::space_local)
			{
				DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetEditorGizmos()->SetLocal();
			}
			else
			{
				DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetEditorGizmos()->SetWorld();
			}
		}
		else
		{
			DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetEditorGizmos()->Hide();
			m_modificatorData->Reset();
		}
		UpdateCameraEditor(dt); 
		UpdateMoveObject();

		//update gizmos
		ZobObject* z = DirectZob::GetInstance()->GetEngine()->GetObjectAt2DCoords(m_lastMouseX, m_lastMouseY, true);
		if (z)
		{
			DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetEditorGizmos()->Select(z);
			DirectZob::LogInfo("On editor object %s", z->GetName());
		}
		else
		{
			DirectZobWrapper::GetWrapper()->GetZobObjectManagerWrapper()->GetEditorGizmos()->UnSelect();
		}

		if (GetInstance() && m_renderWindow && m_running)
		{
			m_renderWindow->Invoke(gcnew Action(this, &CLI::EngineWrapper::UpdateRenderWindowInternal));
		}
	}

	void EngineWrapper::UpdateRenderWindowInternal()
	{
		BufferData* bData = m_Instance->GetBufferData();
		IntPtr p = (IntPtr)bData->buffer;
		int w = bData->width;
		int h = bData->height;
		int renderW = m_renderWindow->Width;
		int renderH = m_renderWindow->Height;
		if (renderH != h || renderW != w)
		{
			m_Instance->Resize(renderW, renderH);
			m_renderWindowGraphics = m_renderWindow->CreateGraphics();
			bData = m_Instance->GetBufferData();
			w = bData->width;
			h = bData->height;
			p = (IntPtr)bData->buffer;
		}
		Bitmap^ b = gcnew Bitmap(w, h, 4 * w, System::Drawing::Imaging::PixelFormat::Format32bppRgb, p);
		Drawing::Rectangle srcRect = Drawing::Rectangle(0, 0, w, h);
		Drawing::Rectangle dstRect = Drawing::Rectangle(0, 0, renderW, renderH);
		m_renderWindow->SizeMode = PictureBoxSizeMode::StretchImage;
		m_renderWindowGraphics->DrawImage(b, dstRect, srcRect, Drawing::GraphicsUnit::Pixel);
	}

	void EngineWrapper::QueueObjectsToRender()
	{
		//OnMouseMove(nullptr, nullptr);
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		BufferData* bData = m_Instance->GetBufferData();
		float w = (float)bData->width / 2.0f;
		float h = (float)bData->height / 2.0f;
		if (c)
		{
			for (int i = 0; i < m_nbTriangles; i++)
			{
				Triangle* t = &m_triangleList[i];
				t->pa->Copy(t->va);
				t->pb->Copy(t->vb);
				t->pc->Copy(t->vc);
				c->ToViewSpace(t->pa);
				c->ToViewSpace(t->pb);
				c->ToViewSpace(t->pc);
				c->GetProjectionMatrix()->Mul(t->pa);
				c->GetProjectionMatrix()->Mul(t->pb);
				c->GetProjectionMatrix()->Mul(t->pc);
				t->pa->x = (t->pa->x / t->pa->z + 1) * w;
				t->pa->y = (t->pa->y / t->pa->z + 1) * h;
				t->pb->x = (t->pb->x / t->pb->z + 1) * w;
				t->pb->y = (t->pb->y / t->pb->z + 1) * h;
				t->pc->x = (t->pc->x / t->pc->z + 1) * w;
				t->pc->y = (t->pc->y / t->pc->z + 1) * h;
				t->draw = true;
				t->material = NULL;
				t->ComputeArea();
				t->clipMode = Triangle::eClip_3_in;
				m_Instance->QueueWorldTriangle(c, t);
			}
		}
		m_nbTriangles = 0;
		for (int i = 0; i < m_nbLines; i++)
		{
			m_Instance->QueueLine(c, &m_lines[i].p0, &m_lines[i].p1, m_lines[i].color, m_lines[i].bold, m_lines[i].noZ);
		}
		m_nbLines = 0;
		for (int i = 0; i < m_nbCircles; i++)
		{
			m_Instance->QueueEllipse(c, &m_circles[i].p, &m_circles[i].n, m_circles[i].r, m_circles[i].r, m_circles[i].color, m_circles[i].bold, m_circles[i].noZ);
		}
		m_nbCircles = 0;
	}

	void EngineWrapper::DrawCircle(ZobVector3* p0, ZobVector3* up, float r, int color, bool bold, bool noZ)
	{
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		if (c)
		{
			if (m_nbCircles < NB_EDITOR_LINES)
			{
				m_circles[m_nbCircles].p = p0;
				m_circles[m_nbCircles].n = up;
				m_circles[m_nbCircles].r = r;
				m_circles[m_nbCircles].color = color;
				m_circles[m_nbCircles].bold = bold;
				m_circles[m_nbCircles].noZ = noZ;
				m_nbCircles++;
			}
		}
	}

	void EngineWrapper::DrawLine(ZobVector3* p0, ZobVector3* p1, int color, bool bold, bool noZ)
	{
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		if (c)
		{
			if (m_nbLines < NB_EDITOR_LINES)
			{
				m_lines[m_nbLines].p0 = p0;
				m_lines[m_nbLines].p1 = p1;
				m_lines[m_nbLines].color = color;
				m_lines[m_nbLines].bold = bold;
				m_lines[m_nbLines].noZ = noZ;
				m_nbLines++;
			}
		}
	}

	void EngineWrapper::OnNewScene()
	{
	}

	void EngineWrapper::OnObjectSelected(ZobObjectWrapper^ z)
	{

	}

	ZobVector3 EngineWrapper::ToScreenCoords(ZobVector3 &v)
	{
		ZobVector3 vout;
		vout.x = (v.x / v.z + 1) * m_renderWindow->Width / 2;
		vout.y = (v.y / v.z + 1) * m_renderWindow->Height / 2;
		vout.z = v.z;
		return vout;
	}

	bool EngineWrapper::IsCursorInsideWindow()
	{
		if (m_renderWindow)
		{
			Point p = Cursor::Position;
			Point w;
			m_renderWindow->PointToScreen(w);
			p.X -= w.X;
			p.Y -= w.Y;
			if (p.X > 0 && p.X < m_renderWindow->Width && p.Y>0 && p.Y < m_renderWindow->Height)
			{
				return true;
			}
		}
		return false;
	}
}
#endif //_WINDLL