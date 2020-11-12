#ifdef _WINDLL
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
		enum class eObjectTypes
		{
			eObjectTypes_all = 0,
			eObjectTypes_editor,
			eObjectTypes_scene,
		};

		ZobObjectManagerWrapper();
		void SaveTransforms();
		void RestoreTransforms();
		System::String^ GetZobObjectList();
		ZobObjectWrapper^ GetZobObject(System::String^ name);
		ZobObjectWrapper^ GetRootObject();
		void RemoveZobObject(ZobObjectWrapper^ name);
		ZobObjectWrapper^ AddZobObject(ZobObjectWrapper^ parent);
		ZobObjectWrapper^ AddZobSprite(ZobObjectWrapper^ parent);
		ZobObjectWrapper^ GetObjectAtCoords(int x, int y, eObjectTypes type);
		void CreateEditorGizmos(System::String^ editorResourcesPath);
		bool Reparent(ZobObjectWrapper^ o, ZobObjectWrapper^ parent);
	private:

	};
}
#endif //_WINDLL