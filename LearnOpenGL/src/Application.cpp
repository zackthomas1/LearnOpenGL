#include "pch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "shader_s.h"
#include "texture_2d.h"
#include "primatives/cube.h"
#include "primatives/plane.h"
#include "delta_time.h"
#include "cameras/fly_camera.h"
#include "cameras/orthographic_camera.h"

#include <assimp/Importer.hpp>

// Declare functions
void ProcessInput(GLFWwindow* window, float time_step);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* CreateGLFWWindow();

Assimp::Importer importer;

// Declare window size constants
const unsigned int kWidth = 800;
const unsigned int kHeight = 600;

// uniform variables 
// adjust texture coordinates in fragment shader
float alpha_tex = 0.5;
float x_pos_tex = 0.0; 
float y_pos_tex = 0.0; 
float scale_tex = 1.0;

//
float last_x = (float)kWidth / 2.0f, last_y = (float)kHeight / 2.0f;
bool first_mouse = true;

LearnOpenGL::FlyCamera camera;
LearnOpenGL::DeltaTime& dt = LearnOpenGL::DeltaTime::getInstance();


//
glm::vec3 cube_positions[] = {
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

int main(void) {
	LearnOpenGL::Log::Init(); 
	TY_CORE_INFO("Initialize Logging");

	// Create window
	GLFWwindow* window = CreateGLFWWindow();
	if (window == nullptr)
		return -1;

	// create shader program
	LearnOpenGL::Shader shader("../assets/shaders/2_lighting_object.vs", "../assets/shaders/2_lighting_object.fs");
	LearnOpenGL::Shader lightCubeShader("../assets/shaders/2_lighting_lightcube.vs", "../assets/shaders/2_lighting_lightcube.fs");

	// define vertex data  
	LearnOpenGL::Plane plane;
	LearnOpenGL::Cube cube;

	// light vertex data
	float light_vertex_data[288] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	unsigned int VBO, lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glGenBuffers(1, &VBO); 

	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(light_vertex_data), light_vertex_data, GL_STATIC_DRAW);
	
	// Light properties
	glm::vec3 lightColor(1.0f);
	//lightColor = lightColor * sin(dt.current_frame_time());
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// SetAttributes ---
	// aPos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0); 

	// aNormal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// aTexCoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load container texture
	LearnOpenGL::Texture2D diffuse_map("../assets/textures/container2.png", false);
	LearnOpenGL::Texture2D specular_map("../assets/textures/container2_specular.png", false);
	LearnOpenGL::Texture2D emission_map("../assets/textures/container2_emission.jpg", false);

	// shader configuration
	shader.Use(); 
	shader.SetInt("material.diffuse", diffuse_map.id() - 1);
	shader.SetInt("material.specular", specular_map.id() - 1);
	shader.SetInt("material.emission", emission_map.id() - 1);

	// Render loop 
	while (!glfwWindowShouldClose(window))
	{
		// rendering commands 
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// update time delta
		dt.Step();

		// input
		ProcessInput(window, dt.delta_time());

		// Light Cube 
		// ----------------------------------------
		// activate shader for light emitting cubes
		lightCubeShader.Use();

		// fragment shader uniforms
		lightCubeShader.SetVec3("emission", lightColor);

		//set model, view, and projection matrices (set uniforms) 
		lightCubeShader.SetMat4("view", camera.GetViewMatrix());
		lightCubeShader.SetMat4("projection", camera.GetProjectionMatrix());

		// draw light cubes
		for (int i = 0; i < sizeof(pointLightPositions) / sizeof(glm::vec3); i++) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f), pointLightPositions[i]);
			lightCubeShader.SetMat4("model", model);

			glBindVertexArray(lightCubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// ----------------------------------------

		// Scene Cubes
		// ----------------------------------------
		// activate shader for non-light objects
		shader.Use();

		// Light caster parameters
		// -----------------------

		// Directional light
		shader.SetVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));

		shader.SetVec3("dirLight.ambient", lightColor * 0.1f);
		shader.SetVec3("dirLight.diffuse", lightColor * 0.5f);
		shader.SetVec3("dirLight.specular", lightColor * 1.0f);

		// Point lights
		for (int i = 0; i < sizeof(pointLightPositions) / sizeof(glm::vec3); i++) {
			char buffer[100];
			
			// Light position
			sprintf(buffer, "pointLights[%d].position", i);
			std::string formatted_string = buffer;
			shader.SetVec3(formatted_string, pointLightPositions[i]);
			
			// Light contribution
			sprintf(buffer, "pointLights[%d].ambient", i);
			formatted_string = buffer;
			shader.SetVec3(formatted_string, lightColor * 0.1f);
			
			sprintf(buffer, "pointLights[%d].diffuse", i);
			formatted_string = buffer;
			shader.SetVec3(formatted_string, lightColor * 0.5f);
			
			sprintf(buffer, "pointLights[%d].specular", i);
			formatted_string = buffer;
			shader.SetVec3(formatted_string, lightColor * 1.0f);

			// attenuation terms
			sprintf(buffer, "pointLights[%d].constant", i);
			formatted_string = buffer;
			shader.SetFloat(formatted_string, 1.0f);
			
			sprintf(buffer, "pointLights[%d].linear", i);
			formatted_string = buffer;
			shader.SetFloat(formatted_string, 0.09f);

			sprintf(buffer, "pointLights[%d].quadratic", i);
			formatted_string = buffer;
			shader.SetFloat(formatted_string, 0.032f);
		}

		// Spot light
		shader.SetVec3("spotLight.position", camera.GetPosition());

		shader.SetVec3("spotLight.ambient", lightColor * 0.1f);
		shader.SetVec3("spotLight.diffuse", lightColor * 0.5f);
		shader.SetVec3("spotLight.specular", lightColor * 1.0f);

		shader.SetFloat("spotLight.constant", 1.0f);
		shader.SetFloat("spotLight.linear", 0.09f);
		shader.SetFloat("spotLight.quadratic", 0.032f);

		shader.SetVec3("spotLight.spotDir", camera.GetForwardVec());
		shader.SetFloat("spotLight.innerAngle", glm::cos(glm::radians(15.0f)));
		shader.SetFloat("spotLight.outerAngle", glm::cos(glm::radians(18.0f)));

		// Camera (position and view-projection)
		// -------------------------------------
		shader.SetVec3("viewPos", camera.GetPosition());
		shader.SetMat4("view", camera.GetViewMatrix()); 
		shader.SetMat4("projection", camera.GetProjectionMatrix());

		// Cube Material parameters
		// ---------------------
		// Bind/activate diffuse map, specular map, emission map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse_map.id());
		specular_map.Activate();
		emission_map.Activate();

		shader.SetFloat("material.shininess", 32.0f);

		// draw call cubes
		for (int i = 0; i < sizeof(cube_positions) / sizeof(glm::vec3); i++)
		{
			// Update model matrix
			glm::mat4 model = glm::translate(glm::mat4(1.0f), cube_positions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			
			if (i % 3 == 0){
				model = glm::rotate(model, dt.current_frame_time() * glm::radians(25.0f), glm::vec3(1.0f, 1.0f, 0.0f));
			}
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			
			shader.SetMat4("model", model);
			cube.Draw();
		}

		// check and call events and swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window); 
	}

	// Terminate window
	glfwTerminate(); 
	return 0;
}

void ProcessInput(GLFWwindow* window, float time_step)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true); 
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		TY_CORE_TRACE("Key W Press");
		camera.ProcessKeyboard(LearnOpenGL::FORWARD, time_step);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		TY_CORE_TRACE("Key S Press");
		camera.ProcessKeyboard(LearnOpenGL::BACKWARD, time_step);
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		TY_CORE_TRACE("Key A Press");
		camera.ProcessKeyboard(LearnOpenGL::LEFT, time_step);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		TY_CORE_TRACE("Key D Press");
		camera.ProcessKeyboard(LearnOpenGL::RIGHT, time_step);
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		TY_CORE_TRACE("Key Q Press");
		camera.ProcessKeyboard(LearnOpenGL::DOWN, time_step);
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		TY_CORE_TRACE("Key E Press");
		camera.ProcessKeyboard(LearnOpenGL::UP, time_step);
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
	if (first_mouse) // initially set to true
	{
		last_x = xpos;
		last_y = ypos;
		first_mouse = false;
	}

	float x_offset = xpos - last_x; 
	float y_offset = ypos - last_y;

	last_x = xpos; 
	last_y = ypos;

	camera.ProcessMouseMovement(x_offset, y_offset);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	camera.ProcessMouseScroll((float)y_offset);
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
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		// capture mouse cursor

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
