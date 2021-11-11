#ifdef _WINDLL
#include "MeshManagerWrapper.h"
#include "../DirectZob.h"

namespace CLI
{
	MeshManagerWrapper::MeshManagerWrapper() :ManagedObject(DirectZob::GetInstance()->GetMeshManager(), false)
	{

	}
	void MeshManagerWrapper::LoadMesh(System::String^ name, System::String^ path, System::String^ file)
	{
		std::string cname;
		std::string cfile;
		std::string cpath;
		MarshalString(name, cname);
		MarshalString(file, cfile);
		MarshalString(path, cpath);
		m_Instance->LoadMesh(cname, cpath, cfile, false);
	}
	System::String^ MeshManagerWrapper::GetMeshList()
	{
		return gcnew System::String(m_Instance->GetMeshList().c_str());
	}
}
#endif //_WINDLL