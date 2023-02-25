#pragma once

#include <string_view>
#include <cinttypes>
#include <concepts>
#include <iostream>
#include <fstream>
#include <format>
#include <string>
#include <array>

#include <glad/glad.h>

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
		Shader(std::string_view vshaderPath, std::string_view fshaderPath);
		Shader();
		~Shader();

		void load(std::string_view vshaderPath, std::string_view fshaderPath);
		void use();
		void write(std::string_view uniform, std::initializer_list<int>&& val);
		void write(std::string_view uniform, std::initializer_list<unsigned int>&& val);
		void write(std::string_view uniform, std::initializer_list<float>&& val);

		//void drawObject();
	};
}