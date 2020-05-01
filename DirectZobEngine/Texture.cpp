#include "Texture.h"
#include "DirectZob.h"
#include "nanojpeg.h"
#include "lodepng.h"
#include "tga.h"

Texture::Texture()
{
	float* m_data = NULL;
	uint m_dataSize = 0;
	uint m_width = 0;
	uint m_height = 0;
	std::string m_fullPath = "";
}

Texture::~Texture()
{
	delete m_data;
	uint m_dataSize = 0;
	uint m_width = 0;
	uint m_height = 0;
	std::string m_fullPath = "";
}

void Texture::LoadFromFile(const std::string& textureFile)
{
	m_fullPath = textureFile;
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;
	uint8_t error = 0;
	std::string name = "";
	if (textureFile.find(".png") != -1)
	{
		error = lodepng::decode(image, width, height, textureFile);
		if (error)
		{
			OnError(error, name.c_str(), textureFile.c_str());
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
			DirectZob::LogInfo("Loaded texture %s", textureFile);
		}
	}
	else if (textureFile.find(".jpg") != -1 || textureFile.find(".jpeg") != -1)
	{
		FILE* f;
		int err = (int)fopen_s(&f, textureFile.c_str(), "rb");
		if (err == 0)
		{
			char* buf;
			fseek(f, 0, SEEK_END);
			int size = (int)ftell(f);
			buf = (char*)malloc(size);
			fseek(f, 0, SEEK_SET);
			size = (int)fread(buf, 1, size, f);
			fclose(f);

			nanojpeg::njInit();
			error = nanojpeg::njDecode(buf, size);
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
				DirectZob::LogInfo("Loaded texture %s", textureFile);
			}
			else
			{
				OnError(error, name.c_str(), textureFile.c_str());
			}
			free((void*)buf);
		}
		else
		{
			OnError(error, name.c_str(), textureFile.c_str());
		}
	}
	else if (textureFile.find(".tga") != -1)
	{
		int w, h, bpp = 0;
		tga::ImageFormat format = tga::ImageFormat::Undefined;
		tga::TGA tgaTex;
		if (tgaTex.Load(textureFile))
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
			DirectZob::LogInfo("Loaded texture %s", textureFile.c_str());
		}
		else
		{
			OnError(error, name.c_str(), textureFile.c_str());
		}
	}
	else
	{
		DirectZob::LogError("no decoder for file %s", textureFile.c_str());
	}
	image.shrink_to_fit();
}


void Texture::OnError(int error, const char* material, const char* texture)
{
	DirectZob::LogError("Error %i occured when loading texture %s for material %s : format not supported", error, texture, material);
}

