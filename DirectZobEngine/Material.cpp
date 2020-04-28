#include "Material.h"
#include "lodepng.h"
#include <iostream>
#include "DirectZob.h"
#include "nanojpeg.h"
#include "tga.h"

Material::Material(const std::string& name, const Vector3* ambientColor, const Vector3* diffuseColor, const char* textureFile /*=NULL*/)
{
	m_name = name.c_str();
	m_ambientColor = ambientColor;
	m_diffuseColor = diffuseColor;
	m_width = 0;
	m_height = 0;
	m_data = NULL;
	if (textureFile)
	{
		std::vector<unsigned char> image; //the raw pixels
		unsigned width, height;
		uint8_t error = 0;
		if (textureFile)
		{
			std::string texFile = std::string(textureFile);
			if (texFile.find(".png") != -1)
			{
				error = lodepng::decode(image, width, height, textureFile);
				if (error)
				{
					OnError(error, name.c_str(), textureFile);
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
			else if (texFile.find(".jpg") != -1 || texFile.find(".jpeg") != -1)
			{
				FILE* f;
				int err = (int)fopen_s(&f, textureFile, "rb");
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
						DirectZob::LogInfo("Loaded texture %s", textureFile);
					}
					else
					{
						OnError(error, name.c_str(), textureFile);
					}
					free((void*)buf);
				}
				else
				{
					OnError(error, name.c_str(), textureFile);
				}
			}
			else if (texFile.find(".tga") != -1)
			{
				int w, h, bpp = 0;
				tga::ImageFormat format = tga::ImageFormat::Undefined;
				tga::TGA tgaTex;
				tgaTex.Load(textureFile);
				//uint8_t* d = NULL;// tga_load(textureFile, &w, &h, &bpp, &error);
				//if (error == 0 && d)
				if(tgaTex.Load(textureFile))
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
					m_data = (float*)malloc(sizeof(float) * w*h*4);
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
						m_data[j + 1] =r;
						m_data[j + 2] = g;
						m_data[j + 3] = a;
						j += 4;
					}
					m_width = w;
					m_height = h;
					m_dataSize = width * height * 4;
					DirectZob::LogInfo("Loaded texture %s", textureFile);
				}
				else
				{
					OnError(error, name.c_str(), textureFile);
				}
			}
			else
			{
				DirectZob::LogError("no decoder for file %s", textureFile);
			}
		}
		image.clear();
	}
	DirectZob::LogInfo("Loaded material %s", name.c_str());
}

void Material::OnError(int error, const char* material, const char* texture)
{
	m_data = NULL;
	m_dataSize = 0;
	m_width = m_height = 0;
	DirectZob::LogError("Error %i occured when loading texture %s for material %s : format not supported", error, texture, material);

}

Material::~Material()
{
	delete m_data;
	m_data = NULL;
	m_name = "deleted";
	m_ambientColor = Vector3(0, 0, 0);
	m_diffuseColor = Vector3(0,0,0);
	m_width = 0;
	m_height = 0;
	m_dataSize = 0;
}
