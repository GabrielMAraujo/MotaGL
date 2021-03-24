#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>
#include <iostream>

#include "../include/stb_image.h"

class Texture {
public:
	unsigned int idArray[2];

	void LoadTexture(std::string texturePath, int index);
};

#endif