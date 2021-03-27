#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/shader.h";
#include "../include/Vec2.h";
#include "../include/Texture.h";

Vec2 movement = Vec2();
float mixAmount = 0.2f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void checkInputs(GLFWwindow* window) {
	//ESC
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	//1
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		//Set Shaded mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//2
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		//Set Wireframe mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//Left arrow key
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		movement.x -= 0.001f * 9;
	}

	//Right arrow key
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		movement.x += 0.001f * 9;
	}

	//Down arrow key
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		movement.y -= 0.001f * 16;
	}

	//Up arrow key
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		movement.y += 0.001f * 16;
	}

	//I
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		if (mixAmount < 1.0f) {
			mixAmount += 0.01f;
		}
	}

	//K
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		if (mixAmount > 0.0f) {
			mixAmount -= 0.01f;
		}
	}
}

unsigned int* initializeObjects() {

	float vertices_1[] = {
		//Rectangle
		//   Position                Color         Texture coodinates
		0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,  // top right
		0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,    0.0f, 1.0f   // top left

		//// first triangle
		//-0.8f, -0.5f, 0.0f, //bottom left
		//-0.2f, -0.5f, 0.0f, //bottom right
		//-0.5f,  0.5f, 0.0f //top middle
	};


	float vertices_2[] = {
		// second triangle
		//     Postions      |     Colors
		 0.2f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f, //bottom left
		 0.8f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f, //bottom right
		 0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f  //top middle
	};

	unsigned int indices_1[] = {
		//Rectangle
		0, 1, 3, //First triangle
		1, 2, 3  //Second triangle

		//Triangle
		//0, 1, 2 //First triangle
		//3, 4, 5  //Second triangle

		//Single triangle
		//0, 1, 2
	};

	unsigned int indices_2[] = {
		//Rectangle
		//0, 1, 3, //First triangle
		//1, 2, 3  //Second triangle

		//Triangle
		0, 1, 2 //First triangle
		//3, 4, 5  //Second triangle
	};

	unsigned int* VBO = new unsigned int[3];
	glGenBuffers(3, VBO);

	//VAO
	unsigned int* VAO = new unsigned int[3];
	glGenVertexArrays(3, VAO);

	//EBO
	unsigned int* EBO = new unsigned int[3];
	glGenBuffers(3, EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_1), indices_1, GL_STATIC_DRAW);

	//Set correct vertex attribute pointers
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_2), indices_2, GL_STATIC_DRAW);

	//Set correct vertex attribute pointers

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_1), indices_1, GL_STATIC_DRAW);

	//Set correct vertex attribute pointers
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return VAO;
}


//TODO - Update to Shader class workflow
//void changeShaderColor(unsigned int shaderProgram) {
//	float time = (float)glfwGetTime();
//	float green = (sin(time) / 2.0f) + 0.5f;
//	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
//	glUseProgram(shaderProgram);
//	glUniform4f(vertexColorLocation, 0.0f, green, 0.0f, 1.0f);
//}

void renderObject(unsigned int VAO, Shader shader, Vec2 movement) {
	//Draw the object
	//changeShaderColor(shaderProgram);
	shader.use();
	shader.setVec2("moveAmount", movement);

	glBindVertexArray(VAO);

	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

glm::mat4 vectorTransformations(int behaviour) {

	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 transformationMatrix = glm::mat4(1.0f);

	if (behaviour == 1) {
		transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.5f, -0.5f, 0.0f));
		//transformationMatrix = glm::rotate(transformationMatrix, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0f));
		transformationMatrix = glm::rotate(transformationMatrix, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0f));
		transformationMatrix = glm::scale(transformationMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	}

	if (behaviour == 2) {
		transformationMatrix = glm::translate(transformationMatrix, glm::vec3(-0.5f, 0.5f, 0.0f));
		transformationMatrix = glm::rotate(transformationMatrix, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0f));
		transformationMatrix = glm::scale(transformationMatrix, glm::vec3(sin((float)glfwGetTime()), cos((float)glfwGetTime()), 0.5f));
	}

	vec = transformationMatrix * vec;

	//std::cout << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]" << std::endl;

	return transformationMatrix;
}

void renderTextureObject(unsigned int VAO, Shader shader, Texture texture, int behaviour) {
	shader.use();
	shader.setFloat("mixAmount", mixAmount);
	shader.setMat4("transform", vectorTransformations(behaviour));


	//Activate and bind each texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.idArray[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture.idArray[1]);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	

	GLFWwindow* window = glfwCreateWindow(1280, 720, "MotaGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(window);

	//GLAD stuff
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 1280, 720);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader* shaderProgram = new Shader[3];
	shaderProgram[0] = Shader("shaders/vertex_transformation.glsl", "shaders/fragment_texture_2.glsl");
	shaderProgram[1] = Shader("shaders/vertex_movement_color.glsl", "shaders/fragment_movement_color.glsl");
	shaderProgram[2] = Shader("shaders/vertex_transformation.glsl", "shaders/fragment_texture_2.glsl");

	float red = 1.0f;

	unsigned int* VAO = initializeObjects();

	//Texture initialization
	Texture texture = Texture();
	texture.LoadTexture("assets/textures/container.jpg", 0);
	texture.LoadTexture("assets/textures/awesomeface.png", 1);

	shaderProgram[0].use();

	//Setting the texture uniforms on texture fragment shader
	shaderProgram[0].setInt("texture1", 0);
	shaderProgram[0].setInt("texture2", 1);

	//Render loop
	while (!glfwWindowShouldClose(window)) {

		//Input routines
		checkInputs(window);

		if (red >= 0.2f) {
			red -= 0.01f;
		}

		//Rendering routines

		//Background
		glClearColor(red, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Objects
		renderTextureObject(VAO[0], shaderProgram[0], texture, 1);
		renderObject(VAO[1], shaderProgram[1], movement);
		renderTextureObject(VAO[2], shaderProgram[2], texture, 2);

		//Frame buffer update
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}