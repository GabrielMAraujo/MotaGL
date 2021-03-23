#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Vec2.h"

class Shader
{
public:
	//Shader program ID
	unsigned int ID;

	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);

	//Use the shader
	void use();

	const void setBool(const std::string& name, bool value);
	const void setInt(const std::string& name, int value);
	const void setFloat(const std::string& name, float value);
	const void setVec2(const std::string& name, Vec2 value);
};

#endif