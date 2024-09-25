#ifdef _WINDLL
#include "ZobMaterialsManagerWrapper.h"
#define TO_MANAGED_STRING(x) gcnew String(x);

namespace DirectZobInterface
{
	Bitmap^ ZobMaterialWrapper::GetTexture()
	{
		if (m_material->GetDiffuseTexture() && m_material->GetDiffuseTexture()->GetData())
		{
			const float* d = m_material->GetDiffuseTexture()->GetData();
			IntPtr p = (IntPtr)&d;
			int l = m_width * m_height * 4;
			cli::array<Byte>^ b = gcnew cli::array<Byte>(l);
			for (int i = 0; i < l; i += 4)
			{
				if (d[i + 3] != 0)
				{
					b[i + 3] = (int)(d[i + 3] * 255.0f);
					b[i + 2] = (int)(d[i] * 255.0f);
					b[i + 1] = (int)(d[i + 1] * 255.0f);
					b[i + 0] = (int)(d[i + 2] * 255.0f);
				}
				else
				{
					b[i + 3] = 255;
					b[i + 2] = 255;
					b[i + 1] = 0;
					b[i + 0] = 255;
				}

			}
			Bitmap^ texture = gcnew Bitmap(m_width, m_height, Imaging::PixelFormat::Format32bppArgb);
			Drawing::Rectangle rect = Drawing::Rectangle(0, 0, m_width, m_height);
			Imaging::BitmapData^ bmpData = texture->LockBits(rect, Imaging::ImageLockMode::ReadWrite, texture->PixelFormat);
			IntPtr ptr = bmpData->Scan0;
			System::Runtime::InteropServices::Marshal::Copy(b, 0, ptr, l);
			texture->UnlockBits(bmpData);
			return texture;
		}
		return nullptr;
	}

	ZobMaterialWrapper::ZobMaterialWrapper(ZobMaterial* zm) :ManagedObject(zm, true)
	{
		m_material = zm;
		m_width = 0;
		m_height = 0;
		m_name = TO_MANAGED_STRING(zm->GetName().c_str());
		DirectZob::LogWarning("tex %s %i", zm->GetName().c_str(), zm->GetDiffuseTexture());
		if (m_material->GetDiffuseTexture() && m_material->GetDiffuseTexture()->GetData())
		{
			const float* d = m_material->GetDiffuseTexture()->GetData();
			IntPtr p = (IntPtr)&d;
			int w = zm->GetDiffuseTexture()->GetWidth();
			int h = zm->GetDiffuseTexture()->GetHeight();
			m_width = w;;
			m_height = h;
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