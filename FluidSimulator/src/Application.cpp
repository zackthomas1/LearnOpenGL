#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(void) {
	std::cout << "Hello World" << std::endl;

	// Initialize GLFW
	glfwInit(); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// 
	int windowWidth = 800; 
	int windowHeight = 600; 

	// Create GLFW Window Object
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl; 
		glfwTerminate(); 
		return -1; 
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to intialize GLAD" << std::endl; 
		return -1;
	}

	// Set rendering window size 
	glViewport(0, 0, windowWidth, windowHeight); 

	// 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Render loop 
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); 

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