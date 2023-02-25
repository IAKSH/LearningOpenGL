#pragma once

#include <string_view>
#include <cinttypes>
#include <concepts>
#include <iostream>
#include <fstream>
#include <format>
#include <string>

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

	template <typename T> concept AnyInitializerList = requires {typename std::initializer_list<T>; };

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

		template <AnyInitializerList T>
		void write(std::string_view uniform, T vals) 
		{
			auto location = glGetUniformLocation(shaderProgram, &uniform.at(0));
			auto printErrorUnknowSize = ()[] {std::cerr << std::format("[ERROR] flat::Shader::write {with T as {}): unknow size of para", typeid(T).name()) << std::endl; };

			if (typeid(T) == typeid(std::initializer_list<int>))
			{
				auto list = dynamic_cast<std::initializer_list<int>*>(vals);

				int len = 0;
				for (auto& item : list) ++len;

				switch (len)
				{
				case 1:
					glUniform1i(location, *(&*list->begin()));
				case 2:
					glUniform2i(location, *(&*list->begin()), *(&*list->begin() + 1));
				case 3:
					glUniform3i(location, *(&*list->begin()), *(&*list->begin() + 1), *(&*list->begin() + 2));
				case 4:
					glUniform4i(location, *(&*list->begin()), *(&*list->begin() + 1), *(&*list->begin() + 2), *(&*list->begin() + 3));
				default:
					printErrorUnknowSize();
					break;
				}
			}
			else if (typeid(T) == typeid(std::initializer_list<unsigned int>))
			{
				using list = vals;

				int len = 0;
				for (auto& item : list) ++len;

				switch (len)
				{
				case 1:
					glUniform1ui(location, *(&*list.begin()));
				case 2:
					glUniform2ui(location, *(&*list.begin()), *(&*list.begin() + 1));
				case 3:
					glUniform3ui(location, *(&*list.begin()), *(&*list.begin() + 1), *(&*list.begin() + 2));
				case 4:
					glUniform4ui(location, *(&*list.begin()), *(&*list.begin() + 1), *(&*list.begin() + 2), *(&*list.begin() + 3));
				default:
					printErrorUnknowSize();
					break;
				}
			}
			else if (typeid(T) == typeid(std::initializer_list<float>))
			{
				using list = vals;

				int len = 0;
				for (auto& item : list) ++len;

				switch (len)
				{
				case 1:
					glUniform1f(location, *(&*list.begin()));
				case 2:
					glUniform2f(location, *(&*list.begin()), *(&*list.begin() + 1));
				case 3:
					glUniform3f(location, *(&*list.begin()), *(&*list.begin() + 1), *(&*list.begin() + 2));
				case 4:
					glUniform4f(location, *(&*list.begin()), *(&*list.begin() + 1), *(&*list.begin() + 2), *(&*list.begin() + 3));
				default:
					printErrorUnknowSize();
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