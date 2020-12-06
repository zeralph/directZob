#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "../ZobObjects/ZobObject.h"
#include "ManagedVector3.h"
#include "ManagedRenderOptions.h"

namespace CLI {
	public ref class ZobObjectWrapper : ManagedObject<ZobObject>
	{
	public:
		ZobObjectWrapper(ZobObject* zobObject);
		~ZobObjectWrapper() override;
		bool					IsValid() override;
		void					SetZobObject(ZobObject* zobObject) { m_Instance = zobObject; }
		System::String^			GetName();
		System::String^			GetFullNodeName();
		void					SetName(System::String^ name);
		ManagedVector3^			GetWorldPosition();
		ManagedVector3^			GetLeft();
		ManagedVector3^			GetForward();
		ManagedVector3^			GetUp();
		ManagedVector3^			GetWorldRotation();
		ManagedVector3^			GetScale();
		ManagedRenderOptions^	GetRenderOptions();
		System::Collections::Generic::List<ZobObjectWrapper^>^ GetChildren();
		void					SetWorldTransform(ManagedVector3^ p);
		void					SetWorldRotation(ManagedVector3^ p);
		void					SetScale(ManagedVector3^ p);
		void					LookAt(ManagedVector3^ forward, ManagedVector3^ left, ManagedVector3^ up, bool addToCurrentRotation);
		System::String^			GetMeshName();
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
		System::String^			FactoryFile();
		void					SaveToFactoryFile(System::String^ file);

		//Physic
		System::String^			GetPhysicComponentType();
		void					SetPhysicComponentType(System::String^ type);
		System::String^			GetPhysicComponentShapeType();
		void					SetPhysicComponentShapeType(System::String^ type);
		void					SetPhysicComponentShapeRadius(float r);
		void					SetPhysicComponentShapeHeight(float h);
		void					SetPhysicComponentShapeHalfExtends(float x, float y, float z);
		void					SetPhysicComponentShapeMesh(System::String^ type);
	protected:
		DirectZobType::guid m_id;
	};
}
#endif //_WINDLL 