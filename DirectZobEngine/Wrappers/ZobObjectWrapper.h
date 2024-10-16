#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "../ZobObjects/ZobObject.h"
#include "ZobUserControls/ZobUserControls.h"
#include "../Components/ZobComponent.h"
#include "../ZobObjects/ZobObject.h"
#using "System.Windows.Forms.dll"
#using "System.dll"
#using "System.Drawing.dll"

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Drawing;

namespace DirectZobInterface {
	public ref class ZobObjectWrapper : ManagedObject<ZobObject>
	{
	public:
		ZobObjectWrapper(ZobObject* zobObject, Panel^ parentPanel, Resources::ResourceManager^ rscMgr);
		~ZobObjectWrapper() override;
		bool					IsValid() override;
		void					SetZobObject(ZobObject* zobObject) { m_Instance = zobObject; }
		String^					GetName();
		String^					GetFullNodeName();
		void					SetName(System::String^ name);
		void					LoadMesh(System::String^ name, System::String^ file, System::String^ path);
		void					LoadSprite(System::String^ name, System::String^ file, System::String^ path);
		void					CreateSprite();
		bool					IsLight();
		bool					IsCamera();
		void					Detach();
		DirectZobType::guid		GetId() { return m_id; }
		void					SaveToFactoryFile(String^ file);
		void					EditorUpdate();
		void					GroupClick(Object^ sender, System::EventArgs^ e);
		void					Refresh();
		bool					Duplicate();
	private:
		ZobObject*				DuplicateInternal(ZobObject* src, ZobObject* parent, int &nbObjs);
		void					CreateObjectview();
		ZobGroupBox^			FillComponentControl(ZobComponent* zb);
		ZobGroupBox^			FillObjectControl(ZobObject* z);
		ZobGroupBox^			FillControlInternal(std::string& name, ZobVariablesExposer* ze, Image^ im);
		void					CreateObjectBeahaviorsView();
		Panel^					m_parentPanel;
		ZobGroupBox^			m_objectPanel;
		ZobGroupBox^			m_objectGroupBox;
		ZobPropertiesContainer^	m_container;
		Resources::ResourceManager^ m_resourcesManager;
		
	protected:
 		unsigned long long		m_id;
		ZobObject*				m_zobObject;
	};
}
#endif //_WINDLL 