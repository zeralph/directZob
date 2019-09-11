#pragma once
#include "ManagedObject.h"
#include "MeshManager.h"
#include "Mesh.h"
#include <string> 
//using namespace System;
namespace CLI
{
	public ref class MeshManagerWrapper : public ManagedObject<MeshManager>
	{
	public:

		MeshManagerWrapper();

		System::String^ GetMeshList();

	private:

	};
}