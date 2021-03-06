#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "../Managers/MeshManager.h"
#include "../Mesh.h"
#include <string> 
//using namespace System;
namespace CLI
{
	public ref class MeshManagerWrapper : public ManagedObject<MeshManager>
	{
	public:

		MeshManagerWrapper();
		void LoadMesh(System::String^ name, System::String^ path, System::String^ file);
		System::String^ GetMeshList();

	private:

	};
}
#endif //_WINDLL