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
		//pin_ptr<int> arrayPin = &m_Instance->GetBufferData()[0];
		//return arrayPin;
	}
	void EngineWrapper::SetRenderOutput(int r)
	{
		if (r >= 0 && r < (int)Engine::RenderOutput::RenderOutputMAX)
		{
			m_Instance->SetRenderOutput((Engine::RenderOutput)r);
		}
	}
}
#endif //_WINDLL