#include "Texture.h"
#include "lodepng.h"
#include <iostream>

Texture::Texture(const std::string& path, const std::string& name, Events* events)
{
	m_name = name.c_str();
	m_events = events;
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;
	m_events->AddEvent(0, "Load texture " + std::string(path));
	//decode
	unsigned error = lodepng::decode(image, width, height, path);

	m_width = width;
	m_height= height;

	//if there's an error, display it
	if (error)
	{
		m_events->AddEvent(0, "decoder error " + std::string(lodepng_error_text(error)));
	}
	else
	{
		m_events->AddEvent(0, "Texture " + std::string(path) + " loaded");
	}

	data = (float*)malloc(sizeof(float) * 4 * image.size());
	for (int i = 0; i < image.size(); i+=4)
	{
		float r = (float)image[i] / 255.0f;
		float g = (float)image[i+1] / 255.0f;
		float b = (float)image[i+2] / 255.0f;
		float a = (float)image[i+3] / 255.0f;
		data[i] = r;
		data[i+1] = g;
		data[i+2] = b;
		data[i+3] = a;
	}

	//the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
}