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
#include "Model.h"
#include "delta_time.h"
#include "cameras/fly_camera.h"
#include "cameras/orthographic_camera.h"

// Declare functions
void ProcessInput(GLFWwindow* window, float time_step);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* CreateGLFWWindow();

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

int main(void) {
    LearnOpenGL::Log::Init(); 
    TY_CORE_INFO("Initialize Logging");

    // Create window
    GLFWwindow* window = CreateGLFWWindow();
    if (window == nullptr)
        return -1;

    // create shader programs
    LearnOpenGL::Shader shader("../assets/shaders/3_model_loading.vs", "../assets/shaders/3_model_loading.fs");
    LearnOpenGL::Shader lightCubeShader("../assets/shaders/2_lighting_lightcube.vs", "../assets/shaders/2_lighting_lightcube.fs");

    //Initialize modles
    LearnOpenGL::Model backpack("../assets/models/backpack/backpack.obj");
    
    // Light properties
    glm::vec3 lightColor(2.0f);

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

        // Scene
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

        // Camera (position and view-projection)
        // -------------------------------------
        shader.SetVec3("viewPos", camera.GetPosition());
        shader.SetMat4("view", camera.GetViewMatrix()); 
        shader.SetMat4("projection", camera.GetProjectionMatrix());

        // backpack model
        // -------------------------------------
        float rotate = glm::radians(45.0f * ((glm::sin(dt.current_frame_time()) + 1) / 2.0f));
        shader.SetMat4("model", glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)), rotate, glm::vec3(0.0f, 1.0f, 0.0f)));
        backpack.Draw(shader);

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
        TY_CORE_TRACE("Key Up Press");
        alpha_tex += 0.1;
        if (alpha_tex > 1.0)
        {
            alpha_tex = 1.0;
        }
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        TY_CORE_TRACE("Key Down Press");
        alpha_tex -= 0.1;
        if (alpha_tex < 0.0)
        {
            alpha_tex = 0.0; 
        }
    }

    if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS)
    {
        TY_CORE_TRACE("Key Left Bracket Press");
        scale_tex += 0.1;

    }
    if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS)
    {
        TY_CORE_TRACE("Key Right Bracket Press");
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
