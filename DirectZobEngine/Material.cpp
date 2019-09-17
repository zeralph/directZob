#include "Material.h"
#include "lodepng.h"
#include <iostream>
#include "DirectZob.h"

Texture::Texture(const std::string& path, const std::string& file, const std::string& name)
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
		data = (float*)malloc(sizeof(float) * 4 * 1);
		float r = (float)236 / 255.0f;
		float g = (float)3 / 255.0f;
		float b = (float)252 / 255.0f;
		float a = (float)255 / 255.0f;
		data[0] = r;
		data[0 + 1] = g;
		data[0 + 2] = b;
		data[0 + 3] = a;
		std::string n = "ERROR loading material ";
		n.append(m_name.c_str());
		DirectZob::LogError(n.c_str());
	}
	else
	{
		data = (float*)malloc(sizeof(float) * 4 * image.size());
		for (int i = 0; i < image.size(); i += 4)
		{
			float r = (float)image[i] / 255.0f;
			float g = (float)image[i + 1] / 255.0f;
			float b = (float)image[i + 2] / 255.0f;
			float a = (float)image[i + 3] / 255.0f;
			data[i] = r;
			data[i + 1] = g;
			data[i + 2] = b;
			data[i + 3] = a;
		}
		std::string n = "Loaded material ";
		n.append(m_name.c_str());
		DirectZob::LogInfo(n.c_str());
	}
}