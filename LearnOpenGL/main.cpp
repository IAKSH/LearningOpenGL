#include <iostream>
#include <cinttypes>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "drawable.hpp"
#include "animation.hpp"

static flat::Shader shader;

flat::Animation ani;

static void loadTexture()
{
	ani.load({ "idk.png","idk1.png" });
	ani.setIntervalMS(1000);
}

static void shaderInitialize()
{
	shader.load("vertexshader.glsl", "fragmentshader.glsl");
	shader.use();
}

flat::Drawable meme;
flat::Drawable meme2;

static void drawInitialize()
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

static void draw()
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

// ---------------------------------------------------

static void initializeGLFW();
static void initializeGLAD();
static void setViewPort();
static void bindGLFWCallBack();
static void mainLoop();

static void frameBufferResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static GLFWwindow* window = nullptr;

int main() noexcept
{
	initializeGLFW();
	initializeGLAD();
	setViewPort();
	bindGLFWCallBack();
	mainLoop();

	return 0;
}

static void mainLoop()
{
	shaderInitialize();
	drawInitialize();

	// test
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	loadTexture();

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

static void bindGLFWCallBack()
{
	glfwSetFramebufferSizeCallback(window, frameBufferResize);
}

static void setViewPort()
{
	glViewport(0, 0, 800, 600);
}

static void initializeGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "[ERROR] Can't initialize GLAD" << std::endl;
		abort();
	}
}

static void initializeGLFW()
{
	// initialize GLFW
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