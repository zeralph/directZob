#ifdef _WINDLL
#include "EngineWrapper.h"

namespace CLI
{
	EngineWrapper::EngineWrapper():ManagedObject(DirectZob::GetInstance()->GetEngine(), false)
	{

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