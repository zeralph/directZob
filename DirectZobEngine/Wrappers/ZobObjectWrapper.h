#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "../ZobObjects/ZobObject.h"
#include "ManagedVector3.h"
#include "ManagedRenderOptions.h"

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
			ComboboxItem(int k, String^ v, const ZobBehavior::wrapperData* w)
			{
				Key = k;
				Value = v;
				w = w;
			}
			String^ ToString() override
			{
				return Value;
			}
			int Key;
			String^ Value;
			const ZobBehavior::wrapperData* w;
	};

	public ref class ZobObjectWrapper : ManagedObject<ZobObject>
	{
	public:
		ZobObjectWrapper(ZobObject* zobObject);
		~ZobObjectWrapper() override;
		bool					IsValid() override;
		void					SetZobObject(ZobObject* zobObject) { m_Instance = zobObject; }
		String^					GetName();
		String^					GetFullNodeName();
		void					SetName(System::String^ name);
		ManagedVector3^			GetWorldPosition();
		ManagedVector3^			GetLeft();
		ManagedVector3^			GetForward();
		ManagedVector3^			GetUp();
		ManagedVector3^			GetWorldRotation();
		ManagedVector3^			GetScale();
		ManagedRenderOptions^	GetRenderOptions();
		UserControl^			FillBehaviorsControl();
		List<ZobObjectWrapper^>^ GetChildren();
		void					SetWorldTransform(ManagedVector3^ p);
		void					SetWorldRotation(ManagedVector3^ p);
		void					SetScale(ManagedVector3^ p);
		void					LookAt(ManagedVector3^ forward, ManagedVector3^ left, ManagedVector3^ up, bool addToCurrentRotation);
		String^					GetMeshName();
		void					SetMesh(System::String^ name);
		void					LoadMesh(System::String^ name, System::String^ file, System::String^ path);
		void					CreateSprite();
		void					SetLightingMode(int lightMode);
		int						GetLightingMode();
		bool					IsLight();
		bool					IsSprite();
		bool					IsCamera();
		bool					HasMesh();
		DirectZobType::guid		GetId() { return m_id; }
		void					SetPhysicComponent(int i);
		bool					IsFromFactoryFile();
		String^					FactoryFile();
		void					SaveToFactoryFile(String^ file);

		//Physic
		void					GetPhysicComponentInfo(System::String^% type, System::String^% shapeType);
		void					SetPhysicComponentInfo(System::String^ type, System::String^ shapeType);
		void					GetPhysicComponentShapeInfo(float% radius, float% height, float% hx, float% hy, float% hz, System::String^% mesh);
		void					SetPhysicComponentShapeInfo(float radius, float height, float hx, float hy, float hz, System::String^ mesh);
		void					GetPhysicComponentColliderInfo(float% bounciness, float% frictionCoeff, float% massDensity, float% RollingResistance);
		void					SetPhysicComponentColliderInfo(float bounciness, float frictionCoeff, float massDensity, float RollingResistance);
		void					SetPhysicComponentScaleWithObject(bool b);
		void					GetPhysicComponentScaleWithObject(bool% b);

		void					FloatHandler(Object^ sender, System::EventArgs^ e);
		void					ListValidationHandler(Object^ sender, System::EventArgs^ e);

	private:
		GroupBox^				FillBehaviorControl(ZobBehavior* zb);
	protected:

		void					AddFloatVariable(TableLayoutPanel^ panel, ZobBehavior::wrapperData* w);
		void					AddEnumVariable(TableLayoutPanel^ panel, ZobBehavior::wrapperData* w);
		ZobBehavior::wrapperData* GetWrapperDataForVariable(String^ variableName);
 		DirectZobType::guid m_id;
	};
}
#endif //_WINDLL 