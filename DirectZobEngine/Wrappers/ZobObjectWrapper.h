#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "../ZobObjects/ZobObject.h"
#include "ManagedVector3.h"
#include "ManagedRenderOptions.h"
#include "ZobUserControls/ZobUserControls.h"
#include "../Behaviors/ZobBehavior.h"
#include "../ZobObjects/ZobObject.h"
#using "System.Windows.Forms.dll"
#using "System.dll"
#using "System.Drawing.dll"

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Drawing;

namespace CLI {

	public ref class ComboboxItem
	{
		public :
			ComboboxItem(int k, String^ v, const ZobVariablesExposer::wrapperData& w)
			{
				Key = k;
				Value = v;
				_w = &(w);
			}
			String^ ToString() override
			{
				return Value;
			}
			int Key;
			String^ Value;
			const ZobVariablesExposer::wrapperData* _w;
	};


	public ref class ZobObjectWrapper : ManagedObject<ZobObject>
	{
	public:
		ZobObjectWrapper(ZobObject* zobObject, Panel^ panel);
		~ZobObjectWrapper() override;
		bool					IsValid() override;
		void					SetZobObject(ZobObject* zobObject) { m_Instance = zobObject; }
		String^					GetName();
		String^					GetFullNodeName();
		void					SetName(System::String^ name);
		ManagedVector3^			GetLocalPosition();
		ManagedVector3^			GetWorldPosition();
		ManagedVector3^			GetLeft();
		ManagedVector3^			GetForward();
		ManagedVector3^			GetUp();
		ManagedVector3^			GetLocalRotation();
		ManagedVector3^			GetWorldRotation();
		ManagedVector3^			GetScale();
		List<ZobObjectWrapper^>^ GetChildren();
		void					SetLocalTransform(ManagedVector3^ p);
		void					SetLocalRotation(ManagedVector3^ p);
		void					SetScale(ManagedVector3^ p);
		void					LookAt(ManagedVector3^ forward, ManagedVector3^ left, ManagedVector3^ up, bool addToCurrentRotation);
		String^					GetMeshName();
		void					SetMesh(System::String^ name);
		void					LoadMesh(System::String^ name, System::String^ file, System::String^ path);
		void					CreateSprite();
		bool					IsLight();
		bool					IsSprite();
		bool					IsCamera();
		bool					HasMesh();
		DirectZobType::guid		GetId() { return m_id; }
		bool					IsFromFactoryFile();
		String^					FactoryFile();
		void					SaveToFactoryFile(String^ file);
		void					EditorUpdate();
		void					GroupClick(Object^ sender, System::EventArgs^ e);

	private:
		void					CreateObjectview();
		ZobGroupBox^			FillBehaviorControl(ZobBehavior* zb);
		ZobGroupBox^			FillObjectControl(ZobObject* z);
		ZobGroupBox^			FillControlInternal(std::string& name, ZobVariablesExposer* ze);
		void					CreateObjectBeahaviorsview();
		Panel^					m_panel;
		ZobGroupBox^			m_objectPanel;
		ZobPropertiesContainer^	m_container;
	protected:

		const ZobVariablesExposer::wrapperData* GetWrapperDataForVariable(String^ variableName);
 		unsigned long long		m_id;
		ZobObject*				m_zobObject;
	};
}
#endif //_WINDLL 