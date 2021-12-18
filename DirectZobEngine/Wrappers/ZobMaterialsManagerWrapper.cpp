#ifdef _WINDLL
#include "ZobMaterialsManagerWrapper.h"
#define TO_MANAGED_STRING(x) gcnew String(x);

namespace CLI
{

	ZobMaterialWrapper::ZobMaterialWrapper(ZobMaterial* zm) :ManagedObject(zm, true) 
	{
		name = TO_MANAGED_STRING(zm->GetName().c_str());
	}

	ZobMaterialsManagerWrapper::ZobMaterialsManagerWrapper():ManagedObject(DirectZob::GetInstance()->GetMaterialManager(), false)
	{


	}

	ZobMaterialsManagerWrapper::~ZobMaterialsManagerWrapper()
	{

	}

	void ZobMaterialsManagerWrapper::ParseMaterials()
	{
		m_materials = gcnew List<ZobMaterialWrapper^>();
		int n = m_Instance->GetNbMaterials();
		for (int i = 0; i < n; i++)
		{
			ZobMaterial* z = m_Instance->GetMaterialNoConst(i);
			ZobMaterialWrapper^ m = gcnew ZobMaterialWrapper(z);
			m_materials->Add(m);
		}
	}

	int ZobMaterialsManagerWrapper::GetNbMaterials()
	{
		return m_Instance->GetNbMaterials();
	}

	ZobMaterialWrapper^ ZobMaterialsManagerWrapper::GetMaterial(int i)
	{
		ZobMaterial* z = m_Instance->GetMaterialNoConst(i);
		ZobMaterialWrapper^ m = gcnew ZobMaterialWrapper(z);
		return m;
	}
}
#endif //_WINDLL