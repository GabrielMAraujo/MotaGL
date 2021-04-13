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

float screenWidth = 1280.0f;
float screenHeight = 720.0f;

Vec2 movement = Vec2();
float mixAmount = 0.2f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float mouseLastX = 640.0f;
float mouseLastY = 360.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);

glm::vec3 cameraDirection;

const float cameraSpeed = 2.5f;
const float mouseSensitivity = 0.1f;

float yaw = -90.0f;
float pitch = 0.0f;
bool firstMouse = false;

float fov = 90;
const float zoomSensitivity = 4.0f;

//Lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

float vertices_1[] = {
	//Rectangle
	//   Position                Color         Texture coodinates
	//0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,  // top right
	//0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,  // bottom right
	//-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.0f,  // bottom left
	//-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,    0.0f, 1.0f   // top left

	//// first triangle
	//-0.8f, -0.5f, 0.0f, //bottom left
	//-0.2f, -0.5f, 0.0f, //bottom right
	//-0.5f,  0.5f, 0.0f //top middle

	//Cube
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse) {
		mouseLastX = xpos;
		mouseLastY = ypos;

		firstMouse = true;
	}

	float xOffset = xpos - mouseLastX;
	float yOffset = ypos - mouseLastY;

	//Applying sensitivity values
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	//Updating yaw and pitch values
	yaw += xOffset;
	pitch += yOffset;

	//Limiting pitch values to avoid invalid head movements
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	//Calculating new direction
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = -sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

	//Updating the values for next frame usage
	mouseLastX = xpos;
	mouseLastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	fov -= (float)yoffset * zoomSensitivity;

	if (fov < 1.0f)
		fov = 1.0f;
	if (fov >= 90.0f)
		fov = 90.0f;
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

	//W
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraFront * cameraSpeed * deltaTime;
	}

	//S
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraFront * cameraSpeed * deltaTime;
	}

	//A
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= cameraRight * cameraSpeed * deltaTime;
	}

	//D
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += cameraRight * cameraSpeed * deltaTime;
	}
}

unsigned int* initializeObjects() {

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

	//Color
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
		//transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.5f, -0.5f, 0.0f));
		////transformationMatrix = glm::rotate(transformationMatrix, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0f));
		//transformationMatrix = glm::rotate(transformationMatrix, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0f));
		//transformationMatrix = glm::scale(transformationMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
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

glm::mat4 updateCamera() {
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraReverseDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraReverseDirection));
	//glm::vec3 cameraUp = glm::cross(cameraReverseDirection, cameraRight);

	//const float radius = 10.0f;
	//float cameraX = sin(glfwGetTime()) * radius;
	//float cameraZ = cos(glfwGetTime()) * radius;

	//glm::mat4 viewMatrix = glm::lookAt(
	//	cameraPos,
	//	cameraTarget,
	//	cameraUp
	//);

	//glm::mat4 viewMatrix = glm::lookAt(
	//	glm::vec3(cameraX, 0.0f, cameraZ),
	//	cameraTarget,
	//	glm::vec3(0.0f, 1.0f, 0.0f)
	//);

	glm::mat4 viewMatrix = glm::lookAt(
		cameraPos,
		cameraPos + cameraFront,
		cameraUp
	);

	return viewMatrix;
}

void createRenderMatrices(Shader shader) {

	shader.use();

	glm::mat4 modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	shader.setMat4("model", modelMatrix);

	glm::mat4 viewMatrix = glm::mat4(1.0);
	//viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
	viewMatrix = updateCamera();
	shader.setMat4("view", viewMatrix);

	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(glm::radians(fov / 2.0f), screenWidth / screenHeight, 0.1f, 100.0f);
	shader.setMat4("projection", projectionMatrix);

}

void renderCubes(unsigned int VAO, Shader shader, Texture texture) {
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	shader.use();
	shader.setFloat("mixAmount", mixAmount);
	shader.setMat4("transform", glm::mat4(1.0f));
	shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	createRenderMatrices(shader);

	//Activate and bind each texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.idArray[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture.idArray[1]);

	glBindVertexArray(VAO);
	//Rendering 10 cubes
	for (unsigned int i = 0; i < 10; i++) {
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		//Setting rotation angle and translation based on index
		modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
		
		float angle = 0.0f;
		
		if (i % 3 == 0) {
			angle = 20.0f * (i + 1);
		}
		
		
		modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		shader.setMat4("model", modelMatrix);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);	
}

void renderTextureObject(unsigned int VAO, Shader shader, Texture texture, int behaviour) {
	shader.use();
	shader.setFloat("mixAmount", mixAmount);
	shader.setMat4("transform", vectorTransformations(behaviour));

	createRenderMatrices(shader);

	//Activate and bind each texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.idArray[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture.idArray[1]);

	glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void updateDeltaTime() {
	float currentTime = glfwGetTime();
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
}

unsigned int createLightObject() {

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//EBO
	unsigned int* EBO = new unsigned int[3];
	glGenBuffers(3, EBO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_1), indices_1, GL_STATIC_DRAW);

	//Binding the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return lightVAO;
}

void renderLightObject(unsigned int lightVAO, Shader lightingShader) {

	lightingShader.use();

	glm::mat4 modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, lightPos);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
	lightingShader.setMat4("model", modelMatrix);

	glm::mat4 viewMatrix = glm::mat4(1.0);
	//viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
	viewMatrix = updateCamera();
	lightingShader.setMat4("view", viewMatrix);

	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::perspective(glm::radians(fov / 2.0f), screenWidth / screenHeight, 0.1f, 100.0f);
	lightingShader.setMat4("projection", projectionMatrix);

	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "MotaGL", NULL, NULL);

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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Shader* shaderProgram = new Shader[4];
	shaderProgram[0] = Shader("shaders/vertex_perspective.glsl", "shaders/fragment_lighting.glsl");
	shaderProgram[1] = Shader("shaders/vertex_movement_color.glsl", "shaders/fragment_movement_color.glsl");
	shaderProgram[2] = Shader("shaders/vertex_transformation.glsl", "shaders/fragment_texture_2.glsl");
	shaderProgram[3] = Shader("shaders/vertex_lighting.glsl", "shaders/fragment_light.glsl");

	float red = 1.0f;

	unsigned int* VAO = initializeObjects();

	unsigned int lightVAO = createLightObject();

	//Texture initialization
	Texture texture = Texture();
	texture.LoadTexture("assets/textures/container.jpg", 0);
	texture.LoadTexture("assets/textures/awesomeface.png", 1);

	shaderProgram[0].use();

	//Setting the texture uniforms on texture fragment shader
	shaderProgram[0].setInt("texture1", 0);
	shaderProgram[0].setInt("texture2", 1);
	createRenderMatrices(shaderProgram[0]);

	//Render loop
	while (!glfwWindowShouldClose(window)) {

		//Input routines
		checkInputs(window);

		//Enabling depth test
		glEnable(GL_DEPTH_TEST);

		if (red >= 0.2f) {
			red -= 0.01f;
		}

		//Rendering routines

		//Background
		glClearColor(red, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Objects
		renderCubes(VAO[0], shaderProgram[0], texture);
		//renderObject(VAO[1], shaderProgram[1], movement);
		//renderTextureObject(VAO[2], shaderProgram[2], texture, 2);
		renderLightObject(VAO[3], shaderProgram[3]);

		//Frame buffer update
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
		updateDeltaTime();
	}
	
	glfwTerminate();
	return 0;
}