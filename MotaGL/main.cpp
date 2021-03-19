#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	vertexColor = vec4(0.5, 1.0, 0.0, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragmentShader2Source = "#version 330 core\n"
"in vec4 vertexColor;"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vertexColor;\n"
"}\0";

const char* fragmentShaderCustomSource = "#version 330 core\n"
"uniform vec4 ourColor;"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = ourColor;\n"
"}\0";

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

}

unsigned int initializeVertexShader(const char* source) {
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &source, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error in vertex shader compilation: \n" << infoLog << std::endl;
	}

	return vertexShader;
}

unsigned int initializeFragmentShader(const char* source) {
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &source, NULL);
	glCompileShader(fragmentShader);

	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Error in fragment shader compilation: \n" << infoLog << std::endl;
	}

	return fragmentShader;
}

unsigned int shaderProgramRoutine(const char* vertexShaderSource, const char* fragmentShaderSource) {
	unsigned int vertexShader = initializeVertexShader(vertexShaderSource);
	unsigned int fragmentShader = initializeFragmentShader(fragmentShaderSource);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Error in shader program linking: \n" << infoLog << std::endl;
	}

	glUseProgram(shaderProgram);

	//Deleting shaders when they are not necessary anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

unsigned int* initializeObjects() {

	float vertices_1[] = {
		//Rectangle
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left

		//// first triangle
		//-0.8f, -0.5f, 0.0f, //bottom left
		//-0.2f, -0.5f, 0.0f, //bottom right
		//-0.5f,  0.5f, 0.0f //top middle
	};


	float vertices_2[] = {
		// second triangle
		 0.2f, -0.5f, 0.0f, //bottom left
		 0.8f, -0.5f, 0.0f, //bottom right
		 0.5f,  0.5f, 0.0f  //top middle
	};

	unsigned int indices_1[] = {
		//Rectangle
		0, 1, 3, //First triangle
		1, 2, 3  //Second triangle

		//Triangle
		//0, 1, 2 //First triangle
		//3, 4, 5  //Second triangle
	};

	unsigned int indices_2[] = {
		//Rectangle
		//0, 1, 3, //First triangle
		//1, 2, 3  //Second triangle

		//Triangle
		0, 1, 2 //First triangle
		//3, 4, 5  //Second triangle
	};

	unsigned int* VBO = new unsigned int[2];
	glGenBuffers(2, VBO);

	//VAO
	unsigned int* VAO = new unsigned int[2];
	glGenVertexArrays(2, VAO);

	//EBO
	unsigned int* EBO = new unsigned int[2];
	glGenBuffers(2, EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_1), indices_1, GL_STATIC_DRAW);

	//Set correct vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_2), indices_2, GL_STATIC_DRAW);

	//Set correct vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return VAO;
}


void changeShaderColor(unsigned int shaderProgram) {
	float time = glfwGetTime();
	float green = (sin(time) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	glUseProgram(shaderProgram);
	glUniform4f(vertexColorLocation, 0.0f, green, 0.0f, 1.0f);
}

void renderObject(unsigned int VAO, unsigned int shaderProgram) {
	//Draw the object
	changeShaderColor(shaderProgram);
	glBindVertexArray(VAO);

	//glDrawArrays(GL_TRIANGLES, 0, 3);
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

	unsigned int* shaderProgram = new unsigned int[2];
	shaderProgram[0] = shaderProgramRoutine(vertexShaderSource, fragmentShaderCustomSource);
	shaderProgram[1] = shaderProgramRoutine(vertexShaderSource, fragmentShaderCustomSource);

	float red = 1.0f;

	unsigned int* VAO = initializeObjects();

	//Render loop
	while (!glfwWindowShouldClose(window)) {

		//Input routines
		checkInputs(window);

		red -= 0.01f;

		//Rendering routines

		//Background
		glClearColor(red, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Objects
		renderObject(VAO[0], shaderProgram[0]);
		renderObject(VAO[1], shaderProgram[1]);

		//Frame buffer update
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}