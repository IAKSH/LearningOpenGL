#include "shader.hpp"

flat::Shader::Shader(std::string_view vshaderPath, std::string_view fshaderPath)
{
	load(vshaderPath, fshaderPath);
}

flat::Shader::Shader()
{

}

flat::Shader::~Shader()
{
	glDeleteShader(shaderProgram);
}

uint32_t flat::Shader::makeShaderProgram(std::string& vshader, std::string& fshader)
{
	uint32_t shaderProgram = glCreateProgram();
	uint32_t vertexShader = compileVertexShader(vshader);
	uint32_t fragmentShader = compileFragmentShader(fshader);

	checkVertexShader(vertexShader);
	checkFragmentShader(fragmentShader);

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

uint32_t flat::Shader::compileVertexShader(std::string_view vshader)
{
	const char* source = &vshader.at(0);
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &source, nullptr);
	glCompileShader(vertexShader);
	return vertexShader;
}

uint32_t flat::Shader::compileFragmentShader(std::string_view fshader)
{
	const char* source = &fshader.at(0);
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &source, nullptr);
	glCompileShader(fragmentShader);
	return fragmentShader;
}

void flat::Shader::checkVertexShader(uint32_t vshaderProgram)
{
	int success;
	char infoLog[512];
	glGetShaderiv(vshaderProgram, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vshaderProgram, sizeof(infoLog), nullptr, infoLog);
		std::cout << "[ERROR] flat::Shader::checkVertexShader | Failed to compile vertex shader:\n" << infoLog << std::endl;
		abort();
	}
}

void flat::Shader::checkFragmentShader(uint32_t fshaderProgram)
{
	int success;
	char infoLog[512];
	glGetShaderiv(fshaderProgram, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fshaderProgram, sizeof(infoLog), nullptr, infoLog);
		std::cout << "[ERROR] flat::Shader::checkFragmentShader | Failed to compile fragment shader:\n" << infoLog << std::endl;
		abort();
	}
}

void flat::Shader::use()
{
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture0"), 0);// "texture0" -> GL_TEXTURE0
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 1);// "texture1" -> GL_TEXTURE1
}

void flat::Shader::load(std::string_view vshaderPath, std::string_view fshaderPath)
{
	std::string vshaderBuffer, fshaderbuffer, readinBuffer;
	std::ifstream ifs(&vshaderPath.at(0), std::ios::in);
	while (!ifs.eof())
	{
		std::getline(ifs, readinBuffer);
		vshaderBuffer += readinBuffer + '\n';
	}

	readinBuffer = "";
	ifs.close();

	ifs.open(&fshaderPath.at(0), std::ios::in);
	while (!ifs.eof())
	{
		std::getline(ifs, readinBuffer);
		fshaderbuffer += readinBuffer + '\n';
	}

	shaderProgram = makeShaderProgram(vshaderBuffer, fshaderbuffer);
}

void flat::Shader::write(std::string_view uniform, std::initializer_list<int>&& val)
{
	auto location = glGetUniformLocation(shaderProgram, uniform.data());
	int len = 0;
	for (auto& item : val) len++;

	switch (len)
	{
	case 1:
		glUniform1i(location, *val.begin());
		break;
	case 2:
		glUniform2i(location, *val.begin(), *(val.begin() + 1));
		break;
	case 3:
		glUniform3i(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2));
		break;
	case 4:
		glUniform4i(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2), *(val.begin() + 3));
		break;
	default:
		std::cerr << std::format("[ERROR] flat::Shader::writeUniformInt | too many/few parameter (int * {}", len) << std::endl;
		abort();
	}
}

void flat::Shader::write(std::string_view uniform, std::initializer_list<unsigned int>&& val)
{
	auto location = glGetUniformLocation(shaderProgram, uniform.data());
	int len = 0;
	for (auto& item : val) len++;

	switch (len)
	{
	case 1:
		glUniform1ui(location, *val.begin());
		break;
	case 2:
		glUniform2ui(location, *val.begin(), *(val.begin() + 1));
		break;
	case 3:
		glUniform3ui(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2));
		break;
	case 4:
		glUniform4ui(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2), *(val.begin() + 3));
		break;
	default:
		std::cerr << std::format("[ERROR] flat::Shader::writeUniformInt | too many/few parameter (unsgined int * {}", len) << std::endl;
		abort();
	}
}

void flat::Shader::write(std::string_view uniform, std::initializer_list<float>&& val)
{
	auto location = glGetUniformLocation(shaderProgram, uniform.data());
	int len = 0;
	for (auto& item : val) len++;

	switch (len)
	{
	case 1:
		glUniform1f(location, *val.begin());
		break;
	case 2:
		glUniform2f(location, *val.begin(), *(val.begin() + 1));
		break;
	case 3:
		glUniform3f(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2));
		break;
	case 4:
		glUniform4f(location, *val.begin(), *(val.begin() + 1), *(val.begin() + 2), *(val.begin() + 3));
		break;
	default:
		std::cerr << std::format("[ERROR] flat::Shader::writeUniformInt | too many/few parameter (float * {}", len) << std::endl;
		abort();
	}
}