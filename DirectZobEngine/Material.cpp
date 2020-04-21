#include "Material.h"
#include "lodepng.h"
#include <iostream>
#include "DirectZob.h"


Material::Material(const std::string& name, const Vector3* ambientColor, const Vector3* diffuseColor, const std::string* textureFile /*=NULL*/)
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
		unsigned error = lodepng::decode(image, width, height, textureFile->c_str());
		if (error)
		{
			m_width = 1;
			m_height = 1;
			m_data = (float*)malloc(sizeof(float) * 4 * 1);
			float r = (float)236 / 255.0f;
			float g = (float)3 / 255.0f;
			float b = (float)252 / 255.0f;
			float a = (float)255 / 255.0f;
			m_data[0] = r;
			m_data[0 + 1] = g;
			m_data[0 + 2] = b;
			m_data[0 + 3] = a;
			std::string n = "ERROR loading texture for material ";
			n.append(m_name.c_str());
			DirectZob::LogError(n.c_str());
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
			std::string n = "Loaded texture for material : ";
			n.append(m_name.c_str());
			n.append(", ");
			n.append(textureFile->c_str());
			DirectZob::LogInfo(n.c_str());
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
}