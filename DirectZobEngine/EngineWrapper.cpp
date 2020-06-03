#ifdef _WINDLL
#include "EngineWrapper.h"
using namespace directZobCLI;

EngineWrapper::EngineWrapper():ManagedObject(directZob::DirectZob::GetInstance()->GetEngine(), false)
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
	if (r >= 0 && r < (int)directZob::eRenderOutput::__eRenderOutput_MAX__)
	{
		m_Instance->SetRenderOutput((directZob::eRenderOutput)r);
	}
}
void EngineWrapper::SetLightingPrecision(int r)
{
	if (r >= 0 && r < (int)directZob::eLightingPrecision::__eLightingPrecision_MAX__)
	{
		m_Instance->SetLightingPrecision((directZob::eLightingPrecision)r);
	}
}
void EngineWrapper::SetRenderMode(int r)
{
	if (r >= 0 && r < (int)directZob::eRenderMode::__eRenderMode_MAX__)
	{
		m_Instance->SetRenderMode((directZob::eRenderMode)r);
	}
}
bool EngineWrapper::GetProjectedCoords(ManagedVector3^ worldSpacePos)
{
	directZob::Vector3 v = worldSpacePos->ToVector3();
	if (m_Instance->GetProjectedCoords(&v))
	{
		worldSpacePos->FromVector3(v);
		return true;
	}
	return false;
}
float EngineWrapper::GetDistanceToCamera(ManagedVector3^ worldSpacePos)
{
	directZob::Vector3 v = worldSpacePos->ToVector3();
	return m_Instance->GetDistanceToCamera(&v);
}

#endif //_WINDLL