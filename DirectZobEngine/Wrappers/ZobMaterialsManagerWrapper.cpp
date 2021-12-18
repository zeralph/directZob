#ifdef _WINDLL
#include "ZobMaterialsManagerWrapper.h"
#define TO_MANAGED_STRING(x) gcnew String(x);

namespace CLI
{

	ZobMaterialWrapper::ZobMaterialWrapper(ZobMaterial* zm) :ManagedObject(zm, true)
	{
		name = TO_MANAGED_STRING(zm->GetName().c_str());
		if (zm->GetDiffuseTexture())
		{
			const float* d = zm->GetDiffuseTexture()->GetData();
			IntPtr p = (IntPtr)&d;
			int w = zm->GetDiffuseTexture()->GetWidth();
			int h = zm->GetDiffuseTexture()->GetWidth();
			int l = w * h * 4;
			_b = gcnew cli::array<Byte>(l);
			for (int i = 0; i < l; i+=4)
			{
				if (d[i + 3] != 0)
				{
					_b[i + 3] = (int)(d[i + 3] * 255.0f);
					_b[i+2] = (int)(d[i] * 255.0f);
					_b[i+1] = (int)(d[i+1] * 255.0f);
					_b[i+0] = (int)(d[i+2] * 255.0f);
				}
				else
				{
					_b[i + 3] = 255;
					_b[i + 2] = 255;
					_b[i + 1] = 0;
					_b[i + 0] = 255;
				}
				
			}
			texture = gcnew Bitmap(w, h, Imaging::PixelFormat::Format32bppArgb);
			Drawing::Rectangle rect = Drawing::Rectangle(0, 0, w, h);
			Imaging::BitmapData^ bmpData = texture->LockBits(rect, Imaging::ImageLockMode::ReadWrite, texture->PixelFormat);
			IntPtr ptr = bmpData->Scan0;
			System::Runtime::InteropServices::Marshal::Copy(_b, 0, ptr, l);
			texture->UnlockBits(bmpData);
		}
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