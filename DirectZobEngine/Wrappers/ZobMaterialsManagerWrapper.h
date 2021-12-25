#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "../Managers/MaterialManager.h"
#include "../ZobMaterial.h"
#include "../DirectZob.h"
#using "System.Windows.Forms.dll"
#using "System.dll"
#using "System.Drawing.dll"

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Drawing;

class DirectZobWrapper;
namespace CLI
{
	public ref class ZobMaterialWrapper : public ManagedObject<ZobMaterial>
	{
	public:
		ZobMaterialWrapper(ZobMaterial* zm);
		~ZobMaterialWrapper(){}
		String^ name;
		Bitmap^ texture;
		//cli::array<Byte>^ _b;
	};

	public ref class ZobMaterialsManagerWrapper : public ManagedObject<MaterialManager>
	{
	public:

		ZobMaterialsManagerWrapper();
		~ZobMaterialsManagerWrapper();
		void							ParseMaterials();
		List<ZobMaterialWrapper^>^		GetMaterialsList() { return m_materials; }
		int								GetNbMaterials();
		ZobMaterialWrapper^				GetMaterial(int i);
	
	private:
		List<ZobMaterialWrapper^>^		m_materials;
	};
}
#endif //_WINDLL