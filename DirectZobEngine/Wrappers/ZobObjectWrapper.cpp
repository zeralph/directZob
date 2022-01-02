#ifdef _WINDLL
#include "DirectZobWrapper.h"
#include "ZobObjectWrapper.h"
#include "../ZobObjects/Light.h"
#include "../ZobObjects/Camera.h"
#include "../DirectZob.h"
#include "../Misc/ZobVariablesExposer.h"
#include "ZobUserControls/ZobUserControls.h"

#define TO_MANAGED_STRING(x) gcnew String(x);

namespace CLI
{
	ZobObjectWrapper::ZobObjectWrapper(ZobObject* zobObject, Panel^ parentPanel):ManagedObject(zobObject, false)
	{
		m_zobObject = zobObject;
		m_id = zobObject->GetIdValue();
		m_parentPanel = parentPanel;
		m_parentPanel->Controls->Clear();
		m_objectPanel = nullptr;
		m_container = nullptr;
		CreateObjectview();
		CreateObjectBeahaviorsView();
	}

	ZobObjectWrapper::~ZobObjectWrapper()
	{
		m_parentPanel->Controls->Remove(m_container);
		m_parentPanel->Controls->Remove(m_objectPanel);
		delete m_objectGroupBox;
		delete m_container;
		delete m_objectPanel;
		m_Instance = NULL;
	}

	void ZobObjectWrapper::Detach()
	{
		m_parentPanel->Controls->Remove(m_container);
		m_parentPanel->Controls->Remove(m_objectPanel);
		m_container = nullptr;
		m_objectPanel = nullptr;
		m_parentPanel = nullptr;
	}

	void ZobObjectWrapper::CreateObjectview()
	{
		if (m_container)
		{
			delete m_container;
		}
		if (m_objectPanel)
		{
			delete m_objectPanel;
		}
		if (m_objectGroupBox)
		{
			delete m_objectGroupBox;
		}
		String^ mStr;
		ZobControlString^ s;
		m_objectPanel = gcnew ZobGroupBox("Object", false);
		m_container = gcnew ZobPropertiesContainer();
		ZobPropertiesContainer^ container = gcnew ZobPropertiesContainer();
		m_objectGroupBox = FillObjectControl(m_zobObject);
		m_container->Controls->Add(m_objectGroupBox);
		m_objectPanel->Controls->Add(m_container);
		m_parentPanel->Controls->Add(m_objectPanel);
	}

	void ZobObjectWrapper::CreateObjectBeahaviorsView()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			const std::vector<ZobComponent*>* behaviors = z->GetComponents();
			if (behaviors->size() > 0)
			{
				for (int i = 0; i < behaviors->size(); i++)
				{
					ZobComponent* zb = behaviors->at(i);
					ZobGroupBox^ behaviorPanel = FillComponentControl(zb);
					m_container->Controls->Add(behaviorPanel);
				}
			}
		}
	}

	ZobGroupBox^ ZobObjectWrapper::FillObjectControl(ZobObject* z)
	{
		if (z)
		{
			std::string name = z->GetName();
			ZobVariablesExposer* v = z->GetVariablesExposer();
			return FillControlInternal(name, v);
		}
		return nullptr;
	}

	ZobGroupBox^ ZobObjectWrapper::FillComponentControl(ZobComponent* zb)
	{
		if (zb)
		{		
			std::string name = zb->GetComponentTypeStr();
			ZobVariablesExposer* v = zb->GetVariablesExposer();
			return FillControlInternal(name, v);
		}
		return nullptr;
	}

	ZobGroupBox^ ZobObjectWrapper::FillControlInternal(std::string& name, ZobVariablesExposer* ze)
	{
		return CLI::ZobControl::CreateWrappedVariablesView(name, ze);
	}

	void ZobObjectWrapper::GroupClick(Object^ sender, EventArgs^ e)
	{
		Button^ btn = (Button^)sender;
		GroupBox^ gb = (GroupBox^)btn->Parent;
		if (gb->Height != 25)
		{
			gb->Height = 25;
			btn->Text = "+";
		}
		else
		{
			int h = 0;
			for (int i = 0; i < gb->Controls->Count; i++)
			{
				h += gb->Controls[i]->Height + 2;
			}
			gb->Height = h;
			btn->Text = "-";
		}
	}

	void ZobObjectWrapper::Refresh()
	{
		CreateObjectview();
		CreateObjectBeahaviorsView();
	}

	void ZobObjectWrapper::EditorUpdate()
	{
	}

	bool ZobObjectWrapper::IsFromFactoryFile()
	{ 
		ZobObject* z = GetInstance();
		if (z)
		{
			return z->IsFromFactoryFile();
		}
		return nullptr;
	}
	String^ ZobObjectWrapper::FactoryFile()
	{ 
		ZobObject* z = GetInstance();
		if (z)
		{
			return TO_MANAGED_STRING(z->FactoryFile().c_str());
		}
		return nullptr;
	}

	void ZobObjectWrapper::SaveToFactoryFile(String^ file)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string f;
			MarshalString(file, f);
			return z->SaveToFactoryFile(f);
		}
	}

	String^ ZobObjectWrapper::GetName()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return TO_MANAGED_STRING(z->GetName().c_str());
		}
		return nullptr;
	}

	String^ ZobObjectWrapper::GetFullNodeName()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string s;
			z->GetFullNodeName(s);
			return TO_MANAGED_STRING(s.c_str());
		}
		return nullptr;
	}


	void ZobObjectWrapper::LoadSprite(String^ name, String^ file, String^ path)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string n;
			MarshalString(name, n);
			std::string f;
			MarshalString(file, f);
			std::string p;
			MarshalString(path, p);
			ZobFilePath zfp = ZobFilePath(n, p, f, true);
			z->LoadSprite(zfp, false);
		}
	}

	void ZobObjectWrapper::LoadMesh(String^ name, String^ file, String^ path)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string n;
			MarshalString(name, n);
			std::string f;
			MarshalString(file, f);
			std::string p;
			MarshalString(path, p);
			ZobFilePath zfp = ZobFilePath(n, p, f, true);
			z->LoadMesh(zfp, false);
		}
	}

	void ZobObjectWrapper::CreateSprite()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			z->CreateSprite();
		}
	}

	void ZobObjectWrapper::SetName(String^ name)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string n;
			MarshalString(name, n);
			z->SetName(n);
		}
	}

	bool ZobObjectWrapper::IsLight()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ZobEntity::ZobSubType t = ZobEntity::GetSubType(z->GetIdValue());
			return t == ZobEntity::ZobSubType::subtype_zobLight;
		}
		return false;
	}

	bool ZobObjectWrapper::IsCamera()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return ZobEntity::GetSubType(z->GetIdValue()) == ZobEntity::ZobSubType::subtype_zobCamera;
		}
		return false;
	}

	bool ZobObjectWrapper::IsValid()
	{
		if (m_Instance)
		{		
			if (DirectZob::GetInstance()->GetZobObjectManager()->IsDeleted(m_id))
			{
				m_Instance = NULL;
				return false;
			}
			else
			{
				return true;
			}
		}
		return false;
	}

	bool ZobObjectWrapper::Duplicate()
	{
		ZobObject* parent = m_Instance->GetParent();
		DirectZob::LogInfo("Duplicating %s under %s", m_Instance->GetName(), parent->GetName());
		DirectZob::eDirectZobLogLevel level = DirectZob::GetLogLevel();
		DirectZob::SetLogLevel(DirectZob::eDirectZobLogLevel_warning);
		int nbObjs = 0;
		ZobObject* newObject = DuplicateInternal(m_Instance, parent, nbObjs);
		std::string newName = std::string(m_Instance->GetName().c_str());
		newName.append("_Copy");
		newObject->SetName(newName);
		ZobVector3 p = newObject->GetWorldPosition();
		p.x += 5;
		p.z += 5;
		newObject->SetWorldPosition(p.x, p.y, p.z);
		DirectZob::SetLogLevel(level);
		DirectZob::LogInfo("Duplicated as %s (and %i objects)", newObject->GetName().c_str(), nbObjs);
		return true;
	}

	ZobObject* ZobObjectWrapper::DuplicateInternal(ZobObject* src, ZobObject* parent, int &nbObjs)
	{
		if (src && !src->IsEditorObject())
		{
			DirectZob::LogWarning("duplicating object %i : %s", nbObjs, src->GetName());
			TiXmlElement* n = (TiXmlElement*)src->SaveUnderNode(n);
			zobId zid = ZobEntity::Regenerate(src->GetIdValue());
			ZobObject* newObject = new ZobObject(zid, n, parent, NULL);
			newObject->Init();
			newObject->SetParent(parent);
			ZobVector3 p = newObject->GetWorldPosition();
			p.x += 1;
			newObject->SetWorldPosition(p.x, p.y, p.z);
			delete n;
			const std::vector<ZobObject*>* c = src->GetChildren();
			nbObjs++;
			for (int i = 0; i < c->size(); i++)
			{
				DuplicateInternal(c->at(i), newObject, nbObjs);
			}
			return newObject;
		}
		return NULL;
	}
}

#endif //_WINDLL