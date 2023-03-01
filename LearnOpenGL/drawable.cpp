#include "drawable.hpp"

#include <glad/glad.h>

#include <iostream>
#include <format>
#include <vector>

flat::Drawable::Drawable()
{

}

flat::Drawable::~Drawable()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vao);
}

void flat::Drawable::make()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	createVBO();
	createEBO();

	// vertex position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// vertex color attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attrib
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void flat::Drawable::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void flat::Drawable::createVBO()
{
	std::vector<float> vertexData;
	for (int i = 0; i < vertexes.size(); i++)
	{
		for (auto& pos : vertexes.at(i))
			vertexData.push_back(pos);
		for (auto& pos : colors.at(i))
			vertexData.push_back(pos);
		for (auto& pos : texCoords.at(i))
			vertexData.push_back(pos);
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &*vertexData.begin(), GL_STATIC_DRAW);
}

void flat::Drawable::createEBO()
{
	const uint32_t indices[] =
	{
		0,1,3,
		1,3,2
	};

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void flat::Drawable::writeVertexes(std::initializer_list<float> vals)
{
	if (vertexes.size() > vals.size())
	{
		std::cerr << std::format("[ERROR] flat::Drawable::writeVertexes > missing required vertex: need {},given {}", vertexes.size() * vertexes.at(0).size(), vals.size()) << std::endl;
		abort();
	}

	auto it = std::begin(vals);
	for (auto& i : vertexes)
	{
		for (auto& j : i)
			j = *(it++);
	}
}

void flat::Drawable::writeTexCoord(std::initializer_list<float> vals)
{
	if (texCoords.size() > vals.size())
	{
		std::cerr << std::format("[ERROR] flat::Drawable::writeTexCoord > missing required coord: need {},given {}", texCoords.size() * texCoords.at(0).size(), vals.size()) << std::endl;
		abort();
	}

	auto it = std::begin(vals);
	for (auto& i : texCoords)
	{
		for (auto& j : i)
			j = *(it++);
	}
}

void flat::Drawable::writeColors(std::initializer_list<float> vals)
{
	if (colors.size() > vals.size())
	{
		std::cerr << std::format("[ERROR] flat::Drawable::writeColors > missing required coord: need {},given {}", colors.size() * colors.at(0).size(), vals.size()) << std::endl;
		abort();
	}

	auto it = std::begin(vals);
	for (auto& i : colors)
	{
		for (auto& j : i)
			j = *(it++);
	}
}
