#pragma once

#include <cinttypes>
#include <string_view>
#include <array>

namespace flat
{
	/*
	class Triangle
	{
	};

	class Rectangle
	{
	private:
		std::array<float, 12> vertices;
		std::array<float, 6> indices;
		
	};

	class DrawableObject
	{
	private:
		
	public:
		DrawableObject();
		~DrawableObject();
	};
	*/

	class Shader
	{
	private:
		uint32_t shaderProgram;

		uint32_t makeShaderProgram(std::string& vshader, std::string& fshader);
		uint32_t compileVertexShader(std::string_view vshader);
		uint32_t compileFragmentShader(std::string_view fshader);
		void checkVertexShader(uint32_t vshaderProgram);
		void checkFragmentShader(uint32_t fshaderProgram);

	public:
		Shader(const char* vshaderPath,const char* fshaderPath);
		Shader();
		~Shader();

		void load(const char* vshaderPath, const char* fshaderPath);
		void use();
		template <typename T>
		void writeUniform(std::string_view uniform, std::initializer_list<T> vals);
		//void drawObject();
		
		// DEBUG ONLY!
		uint32_t getShaderProgram()
		{
			return shaderProgram;
		}
	};
}