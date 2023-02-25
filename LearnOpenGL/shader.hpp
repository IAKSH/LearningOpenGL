#pragma once

#include <string_view>
#include <cinttypes>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
		//void drawObject();

		template <typename T, typename... Args>
		void write(std::string_view uniform, T val, Args... args)
		{
			std::vector<T> buffer(val);

			auto func = [&](T fist) {return; };
			auto func = [&](T fist, Args... args)
			{
				buffer.push_back(fist);
				func(args);
			};

			auto location = glGetUniformLocation(shaderProgram, &uniform.at(0));

			if (typeid(T) == typeid(int))
			{
				switch (buffer.size())
				{
				case 1:
					glUniform1i(location, buffer[0]);
					break;
				case 2:
					glUniform2i(location, buffer[0], buffer[1]);
					break;
				case 3:
					glUniform3i(location, buffer[0], buffer[1], buffer[2]);
					break;
				case 4:
					glUniform4i(location, buffer[0], buffer[1], buffer[2], buffer[3]);
					break;
				default:
					std::cerr << "[ERROR] flat::Shader::write | too many parameter! (int)" << std::endl;
					abort();
					break;
				}
			}
			else if (typeid(T) == typeid(unsigned int))
			{
				switch (buffer.size())
				{
				case 1:
					glUniform1ui(location, buffer[0]);
					break;
				case 2:
					glUniform2ui(location, buffer[0], buffer[1]);
					break;
				case 3:
					glUniform3ui(location, buffer[0], buffer[1], buffer[2]);
					break;
				case 4:
					glUniform4ui(location, buffer[0], buffer[1], buffer[2], buffer[3]);
					break;
				default:
					std::cerr << "[ERROR] flat::Shader::write | too many parameter! (unsigned int)" << std::endl;
					abort();
					break;
				}
			}
			else if (typeid(T) == typeid(float))
			{
				switch (buffer.size())
				{
				case 1:
					glUniform1f(location, buffer[0]);
					break;
				case 2:
					glUniform2f(location, buffer[0], buffer[1]);
					break;
				case 3:
					glUniform3f(location, buffer[0], buffer[1], buffer[2]);
					break;
				case 4:
					glUniform4f(location, buffer[0], buffer[1], buffer[2], buffer[3]);
					break;
				default:
					std::cerr << "[ERROR] flat::Shader::write | too many parameter! (float)" << std::endl;
					abort();
					break;
				}
			}
			else
			{
				std::cerr << "[ERROR] flat::Shader::write | Unknow typeid: name = " << typeid(T).name() << std::endl;
				abort();
			}
		}
	};
}