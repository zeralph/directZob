#ifdef _WINDLL
#include "EngineWrapper.h"

namespace CLI
{
	static DirectZobType::RenderOptions m_renderOptions;

	EngineWrapper::EngineWrapper():ManagedObject(DirectZob::GetInstance()->GetEngine(), false)
	{
		m_renderOptions.zBuffered = false;
		m_renderOptions.bColorize = true;
		m_renderOptions.colorization = ZobVector3(255, 255, 0);
		m_renderOptions.cullMode = eCullMode_None;
		m_triangleList = (Triangle*)malloc(sizeof(Triangle) * NB_EDITOR_TRIANGLES);
		ZobVector3* m_vertices = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_EDITOR_TRIANGLES * 3);
		int vi = 0;
		for (int i = 0; i < NB_EDITOR_TRIANGLES; i ++)
		{
			m_triangleList[i].pa = &m_vertices[vi];
			m_triangleList[i].pb = &m_vertices[vi+1];
			m_triangleList[i].pc = &m_vertices[vi+2];
			m_triangleList[i].options = &m_renderOptions;
			vi += 3;
		}
	}

	EngineWrapper::~EngineWrapper()
	{
		delete m_triangleList;
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

	void EngineWrapper::DrawTriangle(ManagedVector3^ p0, ManagedVector3^ p1, ManagedVector3^ p2, int color)
	{
		/*
		Triangle t = m_triangleList[0];
		ZobVector3 pp0 = p0->ToVector3();
		ZobVector3 pp1 = p1->ToVector3();
		ZobVector3 pp2 = p2->ToVector3();
		t.pa->Copy(&pp0);
		t.pb->Copy(&pp1);
		t.pc->Copy(&pp2);
		t.na = &ZobVector3(0, 1, 0);
		t.nb = &ZobVector3(0, 1, 0);
		t.nc = &ZobVector3(0, 1, 0);
		//t.
//		m_Instance->QueueTriangle(&t);
*/
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