#include "MeshManagerWrapper.h"
#include "DirectZob.h"

namespace CLI
{
	MeshManagerWrapper::MeshManagerWrapper() :ManagedObject(DirectZob::GetInstance()->GetMeshManager(), false)
	{

	}

	System::String^ MeshManagerWrapper::GetMeshList()
	{
		return gcnew System::String(m_Instance->GetMeshList().c_str());
	}
}