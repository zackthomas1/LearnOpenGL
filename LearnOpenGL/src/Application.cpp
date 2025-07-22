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

    //Initialize models
    LearnOpenGL::Model planet("../assets/models/planet/planet.obj");

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));

    uint32_t planet_vbo;
    glGenBuffers(1, &planet_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, planet_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), glm::value_ptr(model), GL_STATIC_DRAW);

    for (uint32_t i = 0; i < planet.meshes.size(); i++) {
        uint32_t vao = planet.meshes[i].VOA_;
        glBindVertexArray(vao);

        // vertex attributes        
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }


    LearnOpenGL::Model asteroid("../assets/models/rock/rock.obj", false);
    uint32_t amount = 2500;
    std::vector<glm::mat4> modelMatrices; 
    modelMatrices.reserve(amount);
    srand(static_cast<unsigned int>(glfwGetTime()));
    float radius = 25.0f;
    float offset = 5.5f; 
    for (uint32_t i = 0; i < amount; i++) {
        glm::mat4 model = glm::mat4(1.0f); 
        
        // 1. translate: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset; 
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x,y,z));

        // 2. scale: scale between 0.05 and 0.25
        float scale = (rand() % 20) / 100.0f + 0.05; 
        model = glm::scale(model, glm::vec3(scale)); 
        
        // 3. rotation: add random rotation around 
        float rotAngle = (rand() % 360); 
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        
        // 4. add to list of matrices 
        modelMatrices.push_back(model);
    }

    uint32_t asteroid_vbo;
    glGenBuffers(1, &asteroid_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, asteroid_vbo);
    // To pass an std::vector to glBufferData, you can use the data() method of the vector, which provides a pointer to the underlying array.  
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * modelMatrices.size(), modelMatrices.data(), GL_STATIC_DRAW);

    for (uint32_t i = 0; i < asteroid.meshes.size(); i++) {
        uint32_t vao = asteroid.meshes[i].VOA_; 
        glBindVertexArray(vao);

        // vertex attributes        
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    // create shader programs
    LearnOpenGL::Shader shader(
        "../assets/shaders/4_10_instancing.vs",
        "../assets/shaders/4_10_instancing.fs"
    );

    //load textures

    
    // Render loop 
    while (!glfwWindowShouldClose(window))
    {
        // update time delta
        dt.Step();

        // input
        ProcessInput(window, dt.delta_time());
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        shader.SetMat4("view",          camera.GetViewMatrix());
        shader.SetMat4("perspective",   camera.GetProjectionMatrix());
        
        planet.Draw(shader);
  
        shader.SetInt("texture_diffuse1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, asteroid.textures_loaded[0].id);
        for (uint32_t i = 0; i < asteroid.meshes.size(); i++)
        {
            glBindVertexArray(asteroid.meshes[i].VOA_); 
            glDrawElementsInstanced(
                GL_TRIANGLES, asteroid.meshes[i].m_indices.size(), GL_UNSIGNED_INT, 0, amount
            );
            glBindVertexArray(0);
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
