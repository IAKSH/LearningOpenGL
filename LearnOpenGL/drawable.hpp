#pragma once

#include <array>

namespace flat
{
	class Drawable
	{
	private:
		std::array<std::array<float, 3>, 4> vertexes;
		std::array<std::array<float, 3>, 4> colors;
		std::array<std::array<float, 2>, 4> texCoords;
		uint32_t vbo, ebo, vao;
		void createVBO();
		void createEBO();
	public:
		Drawable();
		~Drawable();
		void writeVertexes(std::initializer_list<float> vals);
		void writeTexCoord(std::initializer_list<float> vals);
		void writeColors(std::initializer_list<float> vals);
		void make();
		void draw();
	};
}