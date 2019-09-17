#include "Material.h"
#include "lodepng.h"
#include <iostream>
#include "DirectZob.h"

Material::Material(const std::string& path, const std::string& file, const std::string& name)
{
	m_name = name.c_str();
	m_file = file.c_str();
	std::string fullPath = path;
	fullPath.append(file);
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;
	//m_events->AddEvent(0, "Load texture " + std::string(path));
	//decode
	unsigned error = lodepng::decode(image, width, height, fullPath);

	m_width = width;
	m_height= height;

	//if there's an error, display it
	if (error)
	{
		//std::string s = "decoder error " + std::string(lodepng_error_text(error));
		//DirectZob::Log(s);
		m_width = 1;
		m_height = 1;
		//add couleur degueue !
		m_data = (float*)malloc(sizeof(float) * 4 * 1);
		float r = (float)236 / 255.0f;
		float g = (float)3 / 255.0f;
		float b = (float)252 / 255.0f;
		float a = (float)255 / 255.0f;
		m_data[0] = r;
		m_data[0 + 1] = g;
		m_data[0 + 2] = b;
		m_data[0 + 3] = a;
		std::string n = "ERROR loading material ";
		n.append(m_name.c_str());
		DirectZob::LogError(n.c_str());
	}
	else
	{
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
		std::string n = "Loaded material ";
		n.append(m_name.c_str());
		DirectZob::LogInfo(n.c_str());
	}
}