#include "Texture.h"
#include "lodepng.h"
#include <iostream>

Texture::Texture(const char* path)
{
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;
	std::cout << "Load texture " << path;
	//decode
	unsigned error = lodepng::decode(image, width, height, path);

	m_width = width;
	m_height= height;

	//if there's an error, display it
	if (error)
	{
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	}
	else
	{
		std::cout << " OK\n";
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