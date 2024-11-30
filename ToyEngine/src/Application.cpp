#include "pch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "texture_2d.h"
#include "primatives/cube.h"
#include "primatives/plane.h"

// Declare functions
void ProcessInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* CreateGLFWWindow();

// Declare constants
const unsigned int kWidth = 800;
const unsigned int kHeight = 600;

// uniform variables 
// adjust texture coordinates in fragment shader
float alpha_tex = 0.5;
float x_pos_tex = 0.0; 
float y_pos_tex = 0.0; 
float scale_tex = 1.0;

//
glm::mat4 model = glm::mat4(1.0f); 
glm::mat4 view = glm::mat4(1.0f); 
glm::mat4 projection = glm::mat4(1.0f);

int main(void) {
	std::cout << "Hello OpenGL" << std::endl;

	// Create window
	GLFWwindow* window = CreateGLFWWindow();
	if (window == nullptr)
		return -1;

	// create shader program
	ToyEngine::Shader shader("../assets/shaders/hello_triangle.vs", "../assets/shaders/hello_triangle.fs");

	// define vertex data  
	ToyEngine::Plane plane;
	ToyEngine::Cube cube;

	// generate textures
	// ---------
	ToyEngine::Texture2D container_tex("../assets/textures/container.jpg");
	ToyEngine::Texture2D awesome_face_tex("../assets/textures/awesomeface.png", true);
	awesome_face_tex.SetParameters(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

	// tell openGL which texture unit each sampler belongs. (only done once)
	shader.Use();
	shader.SetInt("imgTex0", 0);
	shader.SetInt("imgTex1", awesome_face_tex.id() - 1);	// via texture class

	// Render loop 
	while (!glfwWindowShouldClose(window))
	{
		// input
		ProcessInput(window);

		// rendering commands 
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind texture on corresponding texture units
		container_tex.Activate();
		awesome_face_tex.Activate();

		// update time values
		float time_value = glfwGetTime();
		float periodic_cycle = (sin(time_value) / 2.0f) + 0.5f;

		// set uniforms used in fragment shader
		shader.SetFloat4("periodic_brightness", periodic_cycle, periodic_cycle, periodic_cycle, 1.0f);
		shader.SetFloat("alpha_tex", alpha_tex);
		shader.SetFloat2("pos_tex", x_pos_tex, y_pos_tex);
		shader.SetFloat("scale_tex", scale_tex);

		//Update model, view, projection matrices
		model = glm::mat4(1.0f);
		model = glm::rotate(model, time_value * glm::radians(25.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.75f, 1.75f, 1.75f));

		view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

		projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)kWidth / (float)kHeight, 0.1f, 100.0f);

		// set uniforms 
		shader.SetMat4("model", model);
		shader.SetMat4("view", view); 
		shader.SetMat4("projection", projection);
		
		// set shader program
		shader.Use();

		// draw call cube
		//plane.Draw();
		cube.Draw();

		// check and call events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window); 
	}

	// Terminate window
	glfwTerminate(); 
	return 0;
}

void ProcessInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true); 
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		std::cout << "Key Up Press" << std::endl;
		alpha_tex += 0.1;
		if (alpha_tex > 1.0)
		{
			alpha_tex = 1.0;
		}
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		std::cout << "Key Down Press" << std::endl;
		alpha_tex -= 0.1;
		if (alpha_tex < 0.0)
		{
			alpha_tex = 0.0; 
		}
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		std::cout << "Key W Press" << std::endl;
		y_pos_tex += 0.1;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		std::cout << "Key W Press" << std::endl;
		y_pos_tex -= 0.1;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		std::cout << "Key A Press" << std::endl;
		x_pos_tex -= 0.1;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		std::cout << "Key D Press" << std::endl;
		x_pos_tex += 0.1;
	}
	if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS)
	{
		std::cout << "Key Left Bracket Press" << std::endl;
		scale_tex += 0.1;

	}
	if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS)
	{
		std::cout << "Key Right Bracket Press" << std::endl;
		scale_tex -= 0.1;
		if (scale_tex < 0.0)
			scale_tex = 0.0;
	}
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) 
{
	//std::cout << "xpos: " << xpos << "\typos: " << ypos << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* CreateGLFWWindow()
{
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create GLFW Window Object
	GLFWwindow* window = glfwCreateWindow(kWidth, kHeight, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 	// set call-back function for window resize
	glfwSetKeyCallback(window, key_callback);							// set call-back function key input
	glfwSetCursorPosCallback(window, cursor_position_callback);			// set call-back function cursor pos input

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to intialize GLAD" << std::endl;
		return nullptr;
	}

	// Set rendering window size 
	glViewport(0, 0, kWidth, kHeight);

	return window;
}
