#include "animation.hpp"

#include <iostream>
#include <format>

#include <glad/glad.h>

extern "C"
{
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
}

flat::Animation::Animation()
{
	lastUpdate = std::chrono::high_resolution_clock::now();
}

flat::Animation::Animation(std::initializer_list<std::string_view>&& pathes)
{
	load(pathes);
	Animation();
}

flat::Animation::~Animation()
{
	glDeleteTextures(textures.size(), &textures.at(0));
}

void flat::Animation::setIntervalMS(uint32_t interval)
{
	intervalMS = interval;
}

void flat::Animation::load(std::initializer_list<std::string_view>& pathes)
{
	auto count = pathes.size();

	stbi_set_flip_vertically_on_load(true);
	textures.resize(count);
	glGenTextures(count, &textures.at(0));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int w, h, channels;
	unsigned char* data;
	for (int i = 0; i < count; i++)
	{
		data = stbi_load((pathes.begin() + i)->data(), &w, &h, &channels, 0);
		if (!data)
		{
			std::cerr << std::format("[ERROR] flat::Animation::load > Can't load {}", (pathes.begin() + i)->data()) << std::endl;
			abort();
		}

		glBindTexture(GL_TEXTURE_2D, textures.at(i));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
}

void flat::Animation::load(std::initializer_list<std::string_view>&& pathes)
{
	load(pathes);
}

void flat::Animation::checkUpdate()
{
	auto nowMS = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastUpdate).count() >= intervalMS)
	{
		if (currentIndice + 1 == textures.size())
			currentIndice = 0;
		else
			++currentIndice;
	}
}

const uint32_t& flat::Animation::getCurrentTexture()
{
	return textures.at(currentIndice);
}
