#include "gameplay.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "gameplay.hpp"

void flat::Gameplay::initializeTexture()
{
	ani.load({ "idk.png","idk1.png" });
	ani.setIntervalMS(1000);
}

void flat::Gameplay::initializeShader()
{
	shader.load("vertexshader.glsl", "fragmentshader.glsl");
	shader.use();
}

void flat::Gameplay::initializeDraw()
{
	meme.writeVertexes({ 0.5f,0.5f,0.0f,0.5f,-0.5f,0.0f,-0.5f,-0.5f,0.0f,-0.5f,0.5f,0.0f });
	meme.writeColors({ 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f });
	meme.writeTexCoord({ 1.0f,1.0f, 1.0f,0.0f ,0.0f,0.0f,0.0f,1.0f });
	meme.make();

	meme2.writeVertexes({ 1.0f,1.0f,0.0f,0.0f,-0.36f,0.0f,-0.5f,-0.5f,0.0f,-0.5f,0.5f,0.0f });
	meme2.writeColors({ 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f });
	meme2.writeTexCoord({ 1.0f,1.0f, 1.0f,0.0f ,0.0f,0.0f,0.0f,1.0f });
	meme2.make();
}

void flat::Gameplay::initializeGLFW()
{
	glfwInit();
	glfwInitHint(GLFW_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_VERSION_MINOR, 3);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "Hello OpenGL!", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "[ERROR] Can't create window!" << std::endl;
		abort();
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(3);
}

void flat::Gameplay::initializeGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "[ERROR] Can't initialize GLAD" << std::endl;
		abort();
	}
}

void flat::Gameplay::bindFramebufferSizeCallback()
{
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {glViewport(0, 0, width, height); });
}

void flat::Gameplay::draw()
{
	// update uniform
	shader.write("xyoffset", { static_cast<float>(sin(glfwGetTime())), static_cast<float>(cos(glfwGetTime())) });
	// transform!!!
	glm::mat4 trans(1.0f);
	//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.getShaderProgram(), "transform"), 1, GL_FALSE, glm::value_ptr(trans));

	ani.checkUpdate();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ani.getCurrentTexture());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ani.getCurrentTexture());

	meme.draw();
	meme2.draw();
}

void flat::Gameplay::mainLoop()
{
	// test
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//test openAL
	wava::WavAudio wav("a.wav");
	beginSound(wav, true, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	while (!glfwWindowShouldClose(window))
	{
		// input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		// update

		// draw
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);// clear last frame
		draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

flat::Gameplay::Gameplay()
{

}

flat::Gameplay::~Gameplay()
{
	releaseGLFW();
	releaseOpenAL();
}

void flat::Gameplay::initialize()
{
	initializeGLFW();
	initializeGLAD();
	bindFramebufferSizeCallback();
	initializeOpenAL();
	initializeShader();
	initializeDraw();
	initializeTexture();
}

void flat::Gameplay::initializeOpenAL()
{
	// open defeault device
	device = alcOpenDevice(nullptr); 
	context = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(context);
	alGenSources(1, &audioSource);
}

void flat::Gameplay::releaseGLFW()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void flat::Gameplay::releaseOpenAL()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void flat::Gameplay::beginSound(wava::WavAudio& wav, bool loopable, float posX, float posY, float posZ, float velX, float velY, float velZ)
{
	audioSourcePos[0] = posX;
	audioSourcePos[1] = posY;
	audioSourcePos[2] = posZ;

	audioSourceVel[0] = velX;
	audioSourceVel[1] = velY;
	audioSourceVel[2] = velZ;

	alSourcei(audioSource, AL_BUFFER, wav.getBuffer());
	alSourcef(audioSource, AL_PITCH, 1.0f);
	alSourcef(audioSource, AL_GAIN, 1.0f);
	alSourcefv(audioSource, AL_POSITION, &audioSourcePos.at(0));
	alSourcefv(audioSource, AL_VELOCITY, &audioSourceVel.at(0));
	alSourcei(audioSource, AL_LOOPING, static_cast<ALboolean>(loopable));

	alSourcePlay(audioSource);
}

void flat::Gameplay::stopSounds()
{
	alSourceStop(audioSource);
}