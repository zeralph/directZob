#ifdef _WINDLL
#include "DirectZobWrapper.h"
#include "ZobObjectWrapper.h"
#include "../ZobObjects/Light.h"
#include "../ZobObjects/Camera.h"
#include "../DirectZob.h"
#include "../Misc/ZobVariablesExposer.h"
#include "ZobUserControls/ZobUserControls.h"

#define TO_MANAGED_STRING(x) gcnew String(x);

namespace DirectZobInterface
{
	ZobObjectWrapper::ZobObjectWrapper(ZobObject* zobObject, Panel^ parentPanel, Resources::ResourceManager^ rscMgr):ManagedObject(zobObject, false)
	{
		m_resourcesManager = rscMgr;
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
		m_objectPanel = gcnew ZobGroupBox("Object", nullptr, false);
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
			const std::vector<ZobComponent*>* components = z->GetComponents();
			if (components->size() > 0)
			{
				for (int i = 0; i < components->size(); i++)
				{
					ZobComponent* zb = components->at(i);
					ZobGroupBox^ componentPanel = FillComponentControl(zb);
					m_container->Controls->Add(componentPanel);
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
			Image^ im = (Image^)m_resourcesManager->GetObject("_object");
			return FillControlInternal(name, v, im);
		}
		return nullptr;
	}

	ZobGroupBox^ ZobObjectWrapper::FillComponentControl(ZobComponent* zb)
	{
		if (zb)
		{		
			std::string name = zb->GetComponentTypeStr();
			ZobVariablesExposer* v = zb->GetVariablesExposer();
			ZobEntityInfo^ zi = gcnew ZobEntityInfo(zb);
			if (!zi->_isEditor)
			{
				Image^ im = (Image^)m_resourcesManager->GetObject(zi->_imgName);
				return FillControlInternal(name, v, im);
			}
		}
		return nullptr;
	}

	ZobGroupBox^ ZobObjectWrapper::FillControlInternal(std::string& name, ZobVariablesExposer* ze, Image^ im)
	{
		return DirectZobInterface::ZobControl::CreateWrappedVariablesView(name, ze, im);
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
		m_Instance->Duplicate();
		return true;
	}

	ZobObject* ZobObjectWrapper::DuplicateInternal(ZobObject* src, ZobObject* parent, int &nbObjs)
	{
		assert(false);
		/*
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
		*/
		return NULL;
	}
}

#endif //_WINDLL