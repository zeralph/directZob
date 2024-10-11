#include "ZobTexture.h"
#include "../DirectZob.h"
#include "nanojpeg.h"
#include "lodepng.h"
#include "tga.h"

ZobTexture::ZobTexture()
{
	float* m_data = NULL;
	uint m_dataSize = 0;
	uint m_width = 0;
	uint m_height = 0;
	std::string m_fullPath = "";
}

ZobTexture::~ZobTexture()
{
	//DirectZob::LogInfo("Delete Texture %s", m_fullPath.c_str());
	DirectZob::AddIndent();
	free(m_data);
	uint m_dataSize = 0;
	uint m_width = 0;
	uint m_height = 0;
	std::string m_fullPath = "";
	DirectZob::RemoveIndent();
}

void ZobTexture::LoadFromGlTF(tinygltf::Image& image)
{
	m_name = image.name;
	m_width = image.width;
	m_height = image.height;
	m_dataSize = m_width * m_height * 4;
	m_data = (float*)malloc(sizeof(float) * m_dataSize);
	for (int i = 0; i < m_dataSize; i += 4)
	{
		float r = (float)image.image[i] / 255.0f;
		float g = (float)image.image[i + 1] / 255.0f;
		float b = (float)image.image[i + 2] / 255.0f;
		float a = (float)image.image[i + 3] / 255.0f;
		m_data[i] = r;
		m_data[i + 1] = g;
		m_data[i + 2] = b;
		m_data[i + 3] = a;
	}
}

void ZobTexture::LoadFromData(const std::string& name, int w, int h, unsigned long* data)
{
	m_name = name;
	m_width = w;
	m_height = h;
	m_dataSize = m_width * m_height * 4;
	m_data = (float*)malloc(sizeof(float) * m_dataSize);
	int k = 0;
	for (int i = 0; i < m_dataSize; i += 4)
	{
		if (data[k] != 0)
		{
			int y= 0;
			y++;
		}
		float r = (float)((data[k] & 0xFF000000) >> 24) / 255.0f;
		float g = (float)((data[k] & 0x00FF0000) >> 16) / 255.0f;
		float b = (float)((data[k] & 0x0000FF00) >> 8) / 255.0f;
		float a = (float)(data[k] & 0x000000FF) / 255.0f;
		m_data[i] = r;
		m_data[i + 1] = g;
		m_data[i + 2] = b;
		m_data[i + 3] = a;
		k++;
	}
}

void ZobTexture::LoadFromData(const std::string &name, int w, int h, float* data)
{
	m_name = name;
	m_width = w;
	m_height = h;
	m_data = data;
	m_dataSize = w * h * 4;
}

void ZobTexture::Load(ZobFilePath* zfp)
{
	zfp->LoadData();
	m_name = zfp->GetName();
 	DirectZob::LogInfo("Texture %s creation", m_name.c_str());
	DirectZob::AddIndent();
	std::vector<unsigned char> image; //the raw pixels
	//unsigned char* image = 0;
	unsigned int width, height;
	uint8_t error = 0;
	std::string name = "";
	if (zfp->GetFile().find(".png") != -1 || zfp->GetFile().find(".PNG") != -1)
	{
		lodepng::State state;
		error = (uint8_t)lodepng::decode(image, width, height, state, (unsigned char*)zfp->GetData(), zfp->GetDataLength());
		if (error != 0)
		{
			OnError(error, name.c_str(), m_name.c_str());
		}
		else
		{
			m_width = width;
			m_height = height;
			m_data = (float*)malloc(sizeof(float) * 4 * image.size());
			for (int i = 0; i < image.size(); i += 4)
			{
				float r = (float)image[i] / 255.0f;
				float g = (float)image[i + 1] / 255.0f;
				float b = (float)image[i + 2] / 255.0f;
				float a = (float)image[i + 3] / 255.0f;
				m_data[i] = r;
				m_data[i + 1] = g;
				m_data[i + 2] = b;
				m_data[i + 3] = a;
			}
			m_dataSize = width * height * 4;
			DirectZob::LogInfo("Loaded texture %s", m_name.c_str());
		}
	}
	else if (zfp->GetFile().find(".jpg") != -1 || zfp->GetFile().find(".jpeg") != -1 || zfp->GetFile().find(".JPG") != -1 || zfp->GetFile().find(".JPEG") != -1)
	{
		nanojpeg::njInit();
		error = nanojpeg::njDecode(zfp->GetData(), zfp->GetDataLength());
		if (error == NJ_OK) {
			int s = nanojpeg::njGetImageSize();
			for (int i = 0; i < s; i++)
			{
				image.push_back(nanojpeg::njGetImage()[i]);
			}
			width = nanojpeg::njGetWidth();
			height = nanojpeg::njGetHeight();
			m_width = width;
			m_height = height;
			m_data = (float*)malloc(sizeof(float) * 4 * image.size());
			int j = 0;
			for (int i = 0; i < image.size(); i += 3)
			{
				float r = (float)image[i] / 255.0f;
				float g = (float)image[i + 1] / 255.0f;
				float b = (float)image[i + 2] / 255.0f;
				float a = 1.0f;
				m_data[j] = r;
				m_data[j + 1] = g;
				m_data[j + 2] = b;
				m_data[j + 3] = a;
				j += 4;
			}
			m_dataSize = width * height * 4;
			nanojpeg::njDone();
			DirectZob::LogInfo("Loaded texture %s", m_name.c_str());
		}
		else
		{
			OnError(error, name.c_str(), m_name.c_str());
		}
	}
	else if (zfp->GetFile().find(".tga") != -1 || zfp->GetFile().find(".TGA") != -1)
	{
		int w, h, bpp = 0;
		tga::ImageFormat format = tga::ImageFormat::Undefined;
		tga::TGA tgaTex;
		if (tgaTex.LoadFromStream((char*)zfp->GetData(), zfp->GetDataLength()))
		//if (tgaTex.Load(m_name))
		{
			w = tgaTex.GetWidth();
			h = tgaTex.GetHeight();
			format = tgaTex.GetFormat();
			if (format == tga::ImageFormat::RGB)
			{
				bpp = 3;
			}
			else if (format == tga::ImageFormat::RGBA)
			{
				bpp = 4;
			}
			else if (format == tga::ImageFormat::Monochrome)
			{
				return;// bpp = 1;
			}
			else
			{
				return;
			}
			uint8_t* d = tgaTex.GetData();
			uint64_t s = tgaTex.GetSize();
			m_data = (float*)malloc(sizeof(float) * w * h * 4);
			int j = 0;
			for (int i = 0; i < s; i += bpp)
			{
				float r = (float)d[i] / 255.0f;
				float g = (float)d[i + 1] / 255.0f;
				float b = (float)d[i + 2] / 255.0f;
				float a = 1.0f;
				if (r == 0.0f || g == 0.0f || b == 0.0f)
				{
					int hh = 0;
					hh++;
				}
				if (bpp == 4)
				{
					a = (float)image[i + 3] / 255.0f;
				}
				m_data[j] = b;
				m_data[j + 1] = r;
				m_data[j + 2] = g;
				m_data[j + 3] = a;
				j += 4;
			}
			m_width = w;
			m_height = h;
			m_dataSize = width * height * 4;
			DirectZob::LogInfo("Loaded texture %s", m_name.c_str());
		}
		else
		{
			OnError(error, name.c_str(), m_name.c_str());
		}
	}
	else
	{
		DirectZob::LogError("no decoder for file %s", m_name.c_str());
	}
	image.shrink_to_fit();
	zfp->UnloadData();
	DirectZob::RemoveIndent();
}


void ZobTexture::OnError(int error, const char* material, const char* texture)
{
	DirectZob::LogError("Error %i\noccured when loading texture '%s'\nfor material '%s'", error, texture, material);
}

