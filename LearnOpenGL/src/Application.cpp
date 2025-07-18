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
    LearnOpenGL::Shader shader("../assets/shaders/4_3_blending.vs", "../assets/shaders/4_3_blending.fs");

    //Initialize modles
    LearnOpenGL::Cube cube;
    LearnOpenGL::Plane plane;

    //load textures
    LearnOpenGL::Texture2D cube_texture("../assets/textures/marble.jpg");
    LearnOpenGL::Texture2D floor_texture("../assets/textures/metal.png");
    LearnOpenGL::Texture2D grass_texture("../assets/textures/grass.png", true);
    LearnOpenGL::Texture2D window_texture("../assets/textures/window.png");

    // grass positions 
    std::array<glm::vec3, 5> vegetation = {
        glm::vec3(-1.5f, 0.25f, -0.48f),
        glm::vec3( 1.5f, 0.25f,  0.51f),
        glm::vec3( 0.0f, 0.25f,  0.70f),
        glm::vec3(-0.3f, 0.25f, -2.30f),
        glm::vec3( 0.5f, 0.25f, -0.6f),
    };

    std::array<glm::vec3, 5> windows = {
        glm::vec3( 4.0f, 4.0f, 8.0f),
        glm::vec3(-8.0f, 4.0f,-8.0f),
        glm::vec3( 1.0f, 4.0f, 0.0f),
        glm::vec3( 8.0f, 4.0f, 16.0f),
        glm::vec3(-4.0f, 4.0f,-16.0f),
    };

    shader.Use();
    shader.SetInt("tex", 0);

    // Render loop 
    while (!glfwWindowShouldClose(window))
    {
        // rendering commands 
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        glClearColor(0.1f, 0.1f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // update time delta
        dt.Step();

        // input
        ProcessInput(window, dt.delta_time());

        // Scene
        // ----------------------------------------
        //camera matrice uniforms
        shader.SetMat4("view", camera.GetViewMatrix());
        shader.SetMat4("projection", camera.GetProjectionMatrix());

        // floor
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floor_texture.id());
        shader.SetMat4("model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -20.1f, 0.0f)), glm::vec3(40.0f)));
        cube.Draw();

        // cubes
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, cube_texture.id());
        shader.SetMat4("model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 0.0f)), glm::vec3(2.0f)));
        cube.Draw();

        shader.SetMat4("model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 2.0f, 0.0f)), glm::vec3(4.0f)));
        cube.Draw();

        shader.SetMat4("model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 4.0f, 0.0f)), glm::vec3(8.0f)));
        cube.Draw();

        // grass 
        grass_texture.SetParameters(GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, grass_texture.id());
        for (glm::vec3& veg : vegetation) {
            shader.SetMat4("model", glm::scale(glm::translate(glm::mat4(1.0f), veg), glm::vec3(1.0f)));
            plane.Draw();
        }

        // window

        // When writing to the depth buffer, the depth test does not care if the fragment has transparency or not, 
        // so the transparent parts are written to the depth buffer as any other value.
        // The result is that the background windows are tested on depth as any other opaque object would be, ignoring transparency.
        // Even though the transparent part should show the windows behind it, the depth test discards them.

        // DO NOT break the order
        // 1. Draw all opaque objects first.
        // 2. Sort all the transparent objects.
        // 3. Draw all the transparent objects in sorted order.

        std::map<float, glm::vec3> sorted;  
        for (glm::vec3& window : windows) {
            float distance = glm::length(camera.GetPosition() - window); 
            sorted[distance] = window; 
        }

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, window_texture.id()); 
        for (std::map<float, glm::vec3>::iterator it = sorted.end(); it != sorted.begin();) {
            shader.SetMat4("model", glm::scale(glm::translate(glm::mat4(1.0f), (--it)->second), glm::vec3(8.0f)));
            plane.Draw();
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
