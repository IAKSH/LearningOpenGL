#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <AL/al.h>
#include <AL/alc.h>

#include <array>

#include "audio.hpp"
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

		// unstable code
		ALCdevice* device = nullptr;
		ALCcontext* context = nullptr;
		ALuint audioSource;
		std::array<ALfloat, 3> audioSourcePos;
		std::array<ALfloat, 3> audioSourceVel;

		void initializeTexture();
		void initializeShader();
		void initializeDraw();
		void initializeGLFW();
		void initializeGLAD();
		void initializeOpenAL();
		void bindFramebufferSizeCallback();
		void releaseGLFW();
		void releaseOpenAL();
		void draw();
		// unstable API
		void beginSound(wava::WavAudio& wav, bool loopable, float posX, float posY, float posZ, float velX, float velY, float velZ);
		void stopSounds();
	public:
		Gameplay();
		~Gameplay();
		void initialize();
		void mainLoop();
	};
}