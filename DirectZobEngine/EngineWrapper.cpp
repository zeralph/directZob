#ifdef _WINDLL
#include "EngineWrapper.h"

namespace CLI
{
	static DirectZobType::RenderOptions m_renderOptions;

	EngineWrapper::EngineWrapper():ManagedObject(DirectZob::GetInstance()->GetEngine(), false)
	{
		m_renderOptions.zBuffered = false;
		m_renderOptions.bColorize = false;
		m_renderOptions.bTransparency = true;
		m_renderOptions.colorization = ZobVector3(255, 255, 0);
		m_renderOptions.cullMode = eCullMode_None;
		m_renderOptions.lightMode = DirectZobType::RenderOptions::eLightMode_none;
		m_nbTriangles = 0;
		m_triangleList = (Triangle*)malloc(sizeof(Triangle) * NB_EDITOR_TRIANGLES);
		m_vertices = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_EDITOR_TRIANGLES * 3);
		m_projectedVertices = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_EDITOR_TRIANGLES * 3);
		m_normals = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_EDITOR_TRIANGLES);
		int vi = 0;
		for (int i = 0; i < NB_EDITOR_TRIANGLES; i ++)
		{
			m_triangleList[i].va = &m_vertices[vi];
			m_triangleList[i].vb = &m_vertices[vi+1];
			m_triangleList[i].vc = &m_vertices[vi+2];
			m_triangleList[i].pa = &m_projectedVertices[vi];
			m_triangleList[i].pb = &m_projectedVertices[vi + 1];
			m_triangleList[i].pc = &m_projectedVertices[vi + 2];
			m_triangleList[i].n = &m_normals[i];
			m_triangleList[i].options = &m_renderOptions;
			vi += 3;
		}
	}

	EngineWrapper::~EngineWrapper()
	{
		delete m_triangleList;
		delete m_vertices;
		delete m_projectedVertices;
	}

	int EngineWrapper::GetBufferWidth()
	{
		return m_Instance->GetBufferData()->width;
	}
	int EngineWrapper::GetBufferHeight()
	{
		return m_Instance->GetBufferData()->height;
	}
	System::IntPtr EngineWrapper::GetBufferData()
	{
		return System::IntPtr(m_Instance->GetBufferData()->buffer);
	}
	void EngineWrapper::SetRenderOutput(int r)
	{
		if (r >= 0 && r < (int)eRenderOutput::__eRenderOutput_MAX__)
		{
			m_Instance->SetRenderOutput((eRenderOutput)r);
		}
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
	bool EngineWrapper::GetProjectedCoords(ManagedVector3^ worldSpacePos)
	{
		ZobVector3 v = worldSpacePos->ToVector3();
		if (m_Instance->GetProjectedCoords(&v))
		{
			worldSpacePos->FromVector3(v);
			return true;
		}
		return false;
	}
	float EngineWrapper::GetDistanceToCamera(ManagedVector3^ worldSpacePos)
	{
		ZobVector3 v = worldSpacePos->ToVector3();
		return m_Instance->GetDistanceToCamera(&v);
	} 

	ZobObjectWrapper^ EngineWrapper::GetObjectAt2DCoords(ManagedVector3^ v)
	{
		ZobVector3 p = v->ToVector3();
		ZobObject* z = m_Instance->GetObjectAt2DCoords(&p);
		if(z)
		{
			return gcnew ZobObjectWrapper(z);
		}
		return nullptr;
	}
	void EngineWrapper::DrawLine(ManagedVector3^ p0, ManagedVector3^ p1, int color)
	{
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		if (c)
		{
			ZobVector3 pp0 = p0->ToVector3();
			ZobVector3 pp1 = p1->ToVector3();
			m_Instance->QueueLine(c, &pp0, &pp1, color, false);
		}
	}

	void EngineWrapper::QueueObjectsToRender()
	{
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		BufferData* bData = m_Instance->GetBufferData();
		float w = (float)bData->width / 2.0f;
		float h = (float)bData->height / 2.0f;
		if (c)
		{
			for (int i = 0; i < m_nbTriangles; i++)
			{
				Triangle t = m_triangleList[i];
				t.pa->Copy(t.va);
				t.pb->Copy(t.vb);
				t.pc->Copy(t.vc);
				c->ToViewSpace(t.pa);
				c->ToViewSpace(t.pb);
				c->ToViewSpace(t.pc);
				c->GetProjectionMatrix()->Mul(t.pa);
				c->GetProjectionMatrix()->Mul(t.pb);
				c->GetProjectionMatrix()->Mul(t.pc);
				t.pa->x = (t.pa->x / t.pa->z + 1) * w;
				t.pa->y = (t.pa->y / t.pa->z + 1) * h;
				t.pb->x = (t.pb->x / t.pb->z + 1) * w;
				t.pb->y = (t.pb->y / t.pb->z + 1) * h;
				t.pc->x = (t.pc->x / t.pc->z + 1) * w;
				t.pc->y = (t.pc->y / t.pc->z + 1) * h;
				t.draw = true;
				t.material = NULL;
				t.ComputeArea();
				m_Instance->QueueTriangle(&t);
			}
		}
		m_nbTriangles = 0;
	}

	void EngineWrapper::DrawTriangle(ManagedVector3^ p0, ManagedVector3^ p1, ManagedVector3^ p2, int color)
	{
		Triangle t = m_triangleList[m_nbTriangles];
		ZobVector3 pp0 = p0->ToVector3();
		ZobVector3 pp1 = p1->ToVector3();
		ZobVector3 pp2 = p2->ToVector3();
		t.va->Copy(&pp0);
		t.vb->Copy(&pp1);
		t.vc->Copy(&pp2);
		t.na = &ZobVector3(0, 1, 0);
		t.nb = &ZobVector3(0, 1, 0);
		t.nc = &ZobVector3(0, 1, 0);
		m_nbTriangles++;
		//t.
//		m_Instance->QueueTriangle(&t);
	}

	void EngineWrapper::DrawCircle(ManagedVector3^ p0, ManagedVector3^ up, float r, int color)
	{
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		if (c)
		{
			ZobVector3 pp0 = p0->ToVector3();
			ZobVector3 pup = up->ToVector3();
			ZobMatrix4x4 m;
			m.SetPosition(pp0);
			m_Instance->QueueEllipse(c, &pp0, &pup, r, r, color, true);
		}
	}
}
#endif //_WINDLL