#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_s.h"

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* createGLFWWindow();

int main(void) {
	std::cout << "Hello Triangle" << std::endl;

	GLFWwindow* window = createGLFWWindow();

	// create shader program
	Shader shader("../assets/shaders/hello_triangle.vs", "../assets/shaders/hello_triangle.fs");

	// generate vertex array object (VAO) to store vertex attributes
	unsigned int VAO; 
	glGenVertexArrays(1, &VAO); 
	glBindVertexArray(VAO);

	// specify vertex data  
	float vertices[] = {
		// positions		// colors
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,	// top right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	// bottom left
		-0.5f,  0.5f, 0.0f,	0.0f, 0.0f, 0.0f,	// top left
	}; 

	// create Vertex Buffer Object (VBO)
	unsigned int VBO; 
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0); 

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); 
	glEnableVertexAttribArray(1);

	//  Element Buffer Object (EBO)
	unsigned int indices[] = {
		0,1,3,	// first triangle
		1,2,3,	// second triangle
	};

	unsigned int EBO; 
	glGenBuffers(1, &EBO); 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

	// Render loop 
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); 

		//
		float timeValue = glfwGetTime();
		float brightnessValue = (sin(timeValue) / 2.0f) + 0.5f;

		shader.use();
		shader.setFloat4("ourColor", brightnessValue, brightnessValue, brightnessValue, 1.0f);
		
		//
		glBindVertexArray(VAO); 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// check and call events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window); 
	}

	// Terminate window
	glfwTerminate(); 
	return 0;
}

void processInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true); 
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); 
}

GLFWwindow* createGLFWWindow()
{
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Specify window dimensions
	int windowWidth = 800;
	int windowHeight = 600;

	// Create GLFW Window Object
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to intialize GLAD" << std::endl;
		return false;
	}

	// Set rendering window size 
	glViewport(0, 0, windowWidth, windowHeight);

	// set call-back function for window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}
