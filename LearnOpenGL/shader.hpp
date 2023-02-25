#pragma once

#include <string_view>
#include <cinttypes>
#include <concepts>
#include <iostream>
#include <format>

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

		template <typename T>
		void write(std::string_view uniform, std::initializer_list<T>&& val)
		{
			static_assert(std::is_same<T,int>::value || std::is_same<T, unsigned int>::value || std::is_same<T, float>::value);

			auto reportTypeError = [&]() {std::cerr << std::format("[ERROR] flat::Shader::write | wrong type: typeid(T).name() = {}", typeid(T).name()) << std::endl; };

			auto location = glGetUniformLocation(shaderProgram, uniform.data());
			int len = 0;
			for (auto& item : val)
				len++;

			switch (len)
			{
			case 1:
				if (std::is_same<T, int>::value)
					glUniform1i(location, *val.begin());
				else if (std::is_same<T, unsigned int>::value)
					glUniform1ui(location, *val.begin());
				else if (std::is_same<T, float>::value)
					glUniform1f(location, *val.begin());
				else
					reportTypeError();
				break;
			case 2:
				if (std::is_same<T, int>::value)
					glUniform2i(location, *val.begin(), *(val.begin() + 1));
				else if (std::is_same<T, unsigned int>::value)
					glUniform2ui(location, *val.begin(), *(val.begin() + 1));
				else if (std::is_same<T, float>::value)
					glUniform2f(location, *val.begin(), *(val.begin() + 1));
				else
					reportTypeError();
				break;
			case 3:
				if (std::is_same<T, int>::value)
					glUniform3i(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2));
				else if (std::is_same<T, unsigned int>::value)
					glUniform3ui(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2));
				else if (std::is_same<T, float>::value)
					glUniform3f(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2));
				else
					reportTypeError();
				break;
			case 4:
				if (std::is_same<T, int>::value)
					glUniform4i(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2), *(val.begin() + 3));
				else if (std::is_same<T, unsigned int>::value)
					glUniform4ui(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2), *(val.begin() + 3));
				else if (std::is_same<T, float>::value)
					glUniform4f(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2), *(val.begin() + 3));
				else
					reportTypeError();
				break;
			default:
				std::cerr << std::format("[ERROR] flat::Shader::writeUniformInt | too many/few parameter (unsgined int * {}", len) << std::endl;
				abort();
			}
		}

		//void drawObject();
	};
}