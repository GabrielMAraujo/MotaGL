#include "../include/shader.h"

Shader::Shader(){}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	//Ensure that ifstream objects can throw exceptions
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		//Open files
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);

		//Read files' buffers into streams
		std::stringstream vertexShaderStream, fragmentShaderStream;

		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		//Close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();

		//Populate the strings
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Error: Shader file was not read successfully" << std::endl;
	}

	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();

	//Compile the shaders
	unsigned int vertexShader, fragmentShader;
	int success;
	char infoLog[512];

	//Vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error in vertex shader compilation: \n" << infoLog << std::endl;
	}

	//Fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Error in fragment shader compilation: \n" << infoLog << std::endl;
	}

	//Shader program
	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Error in shader program linking: \n" << infoLog << std::endl;
	}

	//Deleting shaders when they are already linked and not necessary anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


void Shader::use() {
	glUseProgram(ID);
}

const void Shader::setBool(const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

const void Shader::setInt(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

const void Shader::setFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

const void Shader::setVec2(const std::string& name, Vec2 value) {
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
}

const void Shader::setVec3(const std::string& name, float xValue, float yValue, float zValue) {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), xValue, yValue, zValue);
}

const void Shader::setVec3(const std::string& name, glm::vec3 value) {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

const void Shader::setMat4(const std::string& name, glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}