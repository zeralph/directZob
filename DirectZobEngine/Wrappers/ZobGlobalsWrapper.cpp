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
		m_globalsPanel = gcnew ZobGroupBox("Globals", false);
		m_container = gcnew ZobPropertiesContainer();
		ZobGroupBox^ properties = gcnew ZobGroupBox("Properties", false);
		ZobPropertiesContainer^ container = gcnew ZobPropertiesContainer();
		ZobGroupBox^ globalsGroupBox = FillGlobalsControl();
		m_container->Controls->Add(globalsGroupBox);
		m_globalsPanel->Controls->Add(m_container);
		m_parentPanel->Controls->Add(m_globalsPanel);
	}

	ZobGroupBox^ ZobGlobalsWrapper::FillGlobalsControl()
	{
		ZobVariablesExposer* ze = DirectZob::GetInstance()->GetLightManager()->GetVariablesExposer();
		std::string name = std::string("Lighting");
		return CLI::ZobControl::CreateWrappedVariablesView(name, ze);
	}
}

#endif //_WINDLL