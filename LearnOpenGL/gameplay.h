#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "drawable.hpp"
#include "animation.hpp"

// ��Ϊ��������Ϸ�ĳ���Gameplay�Ǻ���ͨ�õ�
// ���������һ����Ϸ��Ӧ���Լ��޸�Gameplay��
// ���ڵ����жඨ�ƣ��ж�ͨ�ã�ȫƾ�Լ�����

namespace flat
{
	class Gameplay
	{
	private:
		flat::Shader shader;
		flat::Animation ani;
		flat::Drawable meme;
		flat::Drawable meme2;
		GLFWwindow* window = nullptr;

		void initializeTexture();
		void initializeShader();
		void initializeDraw();
		void initializeGLFW();
		void initializeGLAD();
		void draw();
	public:
		Gameplay();
		~Gameplay();
		void initialize();
		void mainLoop();
	};
}