#pragma once
#include "ManagedObject.h"
#include "ZobObjectManager.h"
#include "ZobObjectWrapper.h"
#include <string> 
//using namespace System;
namespace CLI
{
	public ref class ZobObjectManagerWrapper : public ManagedObject<ZobObjectManager>
	{
	public:

		ZobObjectManagerWrapper();

		System::String^ GetZobObjectList();
		ZobObjectWrapper^ GetZobObject(System::String^ name);
		void RemoveZobObject(System::String^ name);
		void AddZobObject(System::String^ parent);
		ZobObjectWrapper^ GetObjectAtCoords(int x, int y);
		void CreateEditorGizmos(System::String^ editorResourcesPath);
	private:

	};
}