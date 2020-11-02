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
}
#endif //_WINDLL