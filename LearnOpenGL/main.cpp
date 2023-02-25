#include <iostream>
#include <cinttypes>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "shader.hpp"

// ---------------------------------------------------

static uint32_t texture[2];

static void loadTexture()
{
	stbi_set_flip_vertically_on_load(true);

	const char* path[2] = { "idk.png","idk1.png" };

	glGenTextures(2, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < 2; i++)
	{
		int w, h, channels;
		unsigned char* data = stbi_load(path[i], &w, &h, &channels, 0);
		if (!data)
		{
			std::cerr << "[ERROR] Can't load " << path[i] << std::endl;
			abort();
		}

		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
}

// ---------------------------------------------------

static uint32_t vbo;
static uint32_t vao;
static uint32_t ebo;

static flat::Shader shader;

static void shaderInitialize()
{
	shader.load("vertexshader.glsl", "fragmentshader.glsl");
	shader.use();
}

static void drawInitialize()
{
	// prepare our vertex data
	const float vertexData[] =
	{
		// vertex				color					texCoord
		0.5f,	0.5f,	0.0f,	0.0f,	1.0f, 0.0f,		1.0f,1.0f,	// right-up		0
		0.5f,	-0.5f,	0.0f,	1.0f,	0.0f, 0.0f,		1.0f,0.0f,	// right-down	1
		-0.5f,	-0.5f,	0.0f,	0.0f,	1.0f, 1.0f,		0.0f,0.0f,	// left-down	2
		-0.5f,	0.5f,	0.0f,	0.0f,	0.0f, 1.0f,		0.0f,1.0f	// left-up		3
	};

	// prepare our indices data
	const uint32_t indices[] =
	{
		0,1,3,	// first triangle
		1,3,2	// last
	};

	// create VAO
	glGenVertexArrays(1, &vao);
	// bind VAO
	glBindVertexArray(vao);

	// create VBO
	glGenBuffers(1, &vbo);
	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// copy vertex data to memory (VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// create EBO
	glGenBuffers(1, &ebo);
	// bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	// copy indices data to memory (EBO)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set vertex attrib pointer
	// vertex position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// vertex color attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attrib
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

static void draw()
{
	// update uniform
	shader.write("xyoffset", std::initializer_list<float>{ static_cast<float>(sin(glfwGetTime())), static_cast<float>(cos(glfwGetTime())) });

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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
}