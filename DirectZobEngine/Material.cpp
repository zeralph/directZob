#include "Material.h"
#include "lodepng.h"
#include <iostream>
#include "DirectZob.h"
#include "nanojpeg.h"


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
		unsigned error = 0;
		if (textureFile)
		{
			std::string texFile = std::string(textureFile);
			if (texFile.find(".png") != -1)
			{
				error = lodepng::decode(image, width, height, textureFile);
				if (error)
				{
					m_data = NULL;
					DirectZob::LogError("Error %i occured when loading PNG texture %s for material %s", error, textureFile, name.c_str());
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
					std::string n = "Loaded texture ";
					n.append(textureFile);
					n.append(" for material ");
					n.append(name);
					DirectZob::LogInfo(n.c_str());
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
						std::string n = "Loaded texture ";
						n.append(textureFile);
						n.append(" for material ");
						n.append(name);
						DirectZob::LogInfo(n.c_str());
					}
					else
					{
						m_data = NULL;
						m_dataSize = 0;
						m_width = m_height = 0;
						DirectZob::LogError("Error %i occured when loading JPEG texture %s for material %s", error, textureFile, name.c_str());
					}
					free((void*)buf);
				}
				else
				{
					DirectZob::LogError("Error %i occured when opening JPEG texture %s for material %s", error, textureFile, name.c_str());
					m_data = NULL;
					error = 1;
				}
			}
		}
		image.clear();
	}
	std::string n = "Loaded material ";
	n.append(m_name.c_str());
	DirectZob::LogInfo(n.c_str());
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
