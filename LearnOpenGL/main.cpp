#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cinttypes>

// ---------------------------------------------------

static uint32_t vbo;
static uint32_t vao;
static uint32_t ebo;
static uint32_t shaderProgram;

static void shaderInitialize()
{
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 ourColor;\n"
		"uniform vec2 xyoffset;"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(aPos.x + xyoffset.x,aPos.y + xyoffset.y,aPos.z,1.0);\n"
		"    ourColor = aColor;\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"    FragColor = vec4(ourColor, 1.0);\n"
		"}\0";

	// vertex shader
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// check vertex shader
	int  vertexShaderSuccess;
	char vertexShaderInfoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderSuccess);
	if (!vertexShaderSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, vertexShaderInfoLog);
		std::cout << "[ERROR] Can't compile vertex shader: \n" << vertexShaderInfoLog << std::endl;
		abort();
	}

	// fragment shader
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// check fragment shader
	int  fragmentxShaderSuccess;
	char fragmentShaderInfoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentxShaderSuccess);
	if (!fragmentxShaderSuccess)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, fragmentShaderInfoLog);
		std::cout << "[ERROR] Can't compile fragment shader: \n" << fragmentShaderInfoLog << std::endl;
		abort();
	}

	// link to shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// start shader program
	glUseProgram(shaderProgram);

	// delete vertexShader & fragmentShader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// if you wanna to delete the shader program
	// glDeleteShader(...);
}

static void drawInitialize()
{
	// prepare our vertex data
	const float vertexData[] =
	{
		// vertex				color
		0.5f,	0.5f,	0.0f,	0.0f,	1.0f, 0.0f, // right-up		0
		0.5f,	-0.5f,	0.0f,	1.0f,	0.0f, 0.0f, // right-down	1
		-0.5f,	-0.5f,	0.0f,	0.0f,	1.0f, 1.0f,	// left-down	2
		-0.5f,	0.5f,	0.0f,	0.0f,	0.0f, 1.0f	// left-up		3
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// vertex color attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

static void draw()
{
	// update uniform
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "xyoffset");
	glUniform2f(vertexColorLocation, sin(glfwGetTime()), cos(glfwGetTime()));

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