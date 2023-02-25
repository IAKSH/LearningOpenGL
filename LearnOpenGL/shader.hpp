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

	template <typename T> concept AnyInitializerList = requires{ typename std::initializer_list<T>; };

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

		template <typename T,int n>
		void write(std::string_view uniform, T val) requires AnyInitializerList<T>
		{
			using location = glGetUniformLocation(shaderProgram, &uniform.at(0));
			auto onError = []() {std::cerr << std::format("[ERROR] flat::Shader::write::handler1 | unknow type: typeid(val).name() = {}", typeid(T).name()) << std::endl; abort(); };

			auto handler1 = [&]()
			{
				if (typeid(val) == typeid(std::initializer_list<int>))
					glUniform1i(location, *val.begin());
				else if (typeid(val) == typeid(std::initializer_list<unsigned int>))
					glUniform1ui(location, *val.begin());
				else if (typeid(val) == typeid(std::initializer_list<float>))
					glUniform1f(location, *val.begin());
				else
					onError();
			};

			auto handler2 = [&]()
			{
				if (typeid(val) == typeid(std::initializer_list<int>))
					glUniform2i(location, *val.begin(), *(val.begin() + 1));
				else if (typeid(val) == typeid(std::initializer_list<unsigned int>))
					glUniform2ui(location, *val.begin(), *(val.begin() + 1));
				else if (typeid(val) == typeid(std::initializer_list<float>))
					glUniform2f(location, *val.begin(), *(val.begin() + 1));
				else
					onError();
			};

			auto handler3 = [&]()
			{
				if (typeid(val) == typeid(std::initializer_list<int>))
					glUniform3i(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2));
				else if (typeid(val) == typeid(std::initializer_list<unsigned int>))
					glUniform3ui(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2));
				else if (typeid(val) == typeid(std::initializer_list<float>))
					glUniform3f(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2));
				else
					onError();
			};

			auto handler4 = [&]()
			{
				if (typeid(val) == typeid(std::initializer_list<int>))
					glUniform4i(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2), *(val.begin() + 3));
				else if (typeid(val) == typeid(std::initializer_list<unsigned int>))
					glUniform4ui(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2), *(val.begin() + 3));
				else if (typeid(val) == typeid(std::initializer_list<float>))
					glUniform4f(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2), *(val.begin() + 3));
				else
					onError();
			};

			//...

		}

		//void drawObject();
	};
}