#ifndef DZOB_MESH_MANAGER_WRAPPER_H
#define DZOB_MESH_MANAGER_WRAPPER_H
#ifdef _WINDLL

#include "ManagedObject.h"
#include "MeshManager.h"
#include "Mesh.h"
#include <string> 
namespace directZobCLI
{
	public ref class MeshManagerWrapper : public ManagedObject<directZob::MeshManager>
	{
	public:

		MeshManagerWrapper();
		void LoadMesh(System::String^ name, System::String^ path, System::String^ file);
		System::String^ GetMeshList();

	private:

	};
}
#endif //_WINDLL
#endif //DZOB
