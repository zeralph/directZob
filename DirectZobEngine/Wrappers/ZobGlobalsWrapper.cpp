#ifdef _WINDLL
#include "DirectZobWrapper.h"
#include "ZobGlobalsWrapper.h"
#include "../DirectZob.h"
#include "../Misc/ZobVariablesExposer.h"
#include "ZobUserControls/ZobUserControls.h"

#define TO_MANAGED_STRING(x) gcnew String(x);

namespace CLI
{
	ZobGlobalsWrapper::ZobGlobalsWrapper(Panel^ parentPanel)
	{
		m_parentPanel = parentPanel;
		m_parentPanel->Controls->Clear();
		m_globalsPanel = nullptr;
		m_container = nullptr;
		CreateGlobalView();
	}
	ZobGlobalsWrapper::~ZobGlobalsWrapper()
	{

	}

	void ZobGlobalsWrapper::CreateGlobalView()
	{
		if (m_container)
		{
			delete m_container;
		}
		if (m_globalsPanel)
		{
			delete m_globalsPanel;
		}
		String^ mStr;
		ZobControlString^ s;
		m_globalsPanel = gcnew ZobGroupBox("Globals", nullptr, false);
		m_container = gcnew ZobPropertiesContainer();
		ZobGroupBox^ properties = gcnew ZobGroupBox("Properties", nullptr, false);
		ZobPropertiesContainer^ container = gcnew ZobPropertiesContainer();
		ZobGroupBox^ sceneGroupBox = FillSceneControl();
		ZobGroupBox^ engineGroupBox = FillEngineControl();
		m_container->Controls->Add(sceneGroupBox);
		m_container->Controls->Add(engineGroupBox);
		m_globalsPanel->Controls->Add(m_container);
		m_parentPanel->Controls->Add(m_globalsPanel);
	}

	ZobGroupBox^ ZobGlobalsWrapper::FillSceneControl()
	{
		ZobVariablesExposer* ze = DirectZob::GetInstance()->GetLightManager()->GetVariablesExposer();
		std::string name = std::string("Lighting");
		return CLI::ZobControl::CreateWrappedVariablesView(name, ze, nullptr);
	}

	ZobGroupBox^ ZobGlobalsWrapper::FillEngineControl()
	{
		ZobVariablesExposer* ze = DirectZob::GetInstance()->GetEngine()->GetVariablesExposer();
		std::string name = std::string("Engine");
		return CLI::ZobControl::CreateWrappedVariablesView(name, ze, nullptr);
	}
}

#endif //_WINDLL