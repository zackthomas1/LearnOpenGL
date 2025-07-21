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
    LearnOpenGL::Shader skybox_shader("../assets/shaders/4_6_skybox.vs", "../assets/shaders/4_6_skybox.fs");
    LearnOpenGL::Shader shader("../assets/shaders/4_6_reflection_map.vs", "../assets/shaders/4_6_reflection_map.fs");
    LearnOpenGL::Shader framebuffer_shader("../assets/shaders/4_5_framebuffer.vs", "../assets/shaders/4_5_framebuffer.fs");

    shader.Use();
    shader.SetInt("tex", 0);
    shader.SetInt("skybox", 1);

    framebuffer_shader.Use(); 
    framebuffer_shader.SetInt("screenTexture", 0);

    skybox_shader.Use();
    skybox_shader.SetInt("skybox", 0);

    //Initialize models
    LearnOpenGL::Cube cube;

    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    uint32_t quad_vao, quad_vbo;
    glGenVertexArrays(1, &quad_vao);
    glGenBuffers(1, &quad_vbo);
    glBindVertexArray(quad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    uint32_t skybox_vao, skybox_vbo;
    glGenVertexArrays(1, &skybox_vao);
    glGenBuffers(1, &skybox_vbo);
    glBindVertexArray(skybox_vao);
    glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //load textures
    LearnOpenGL::Texture2D cube_texture("../assets/textures/marble.jpg");
    LearnOpenGL::Texture2D floor_texture("../assets/textures/metal.png");

    uint32_t skybox_id;
    glGenTextures(1, &skybox_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_id);

    std::array<const char*, 6> skybox_faces = {
        "../assets/skybox/right.jpg",   // +X
        "../assets/skybox/left.jpg",    // -X
        "../assets/skybox/top.jpg",     // +Y
        "../assets/skybox/bottom.jpg",  // -Y
        "../assets/skybox/front.jpg",   // +Z
        "../assets/skybox/back.jpg",    // -Z
    };
    int width, height, nrChannels;
    for (uint32_t i = 0; i < skybox_faces.size(); i++) {
        unsigned char* data = stbi_load(skybox_faces[i], &width, &height, &nrChannels, 0);
        if (data) {
            TY_CORE_INFO("Loaded cubemap face {}: {}x{}, {} channels", i, width, height, nrChannels);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else {
            TY_CORE_ERROR("Cubemap tex failed to load at path {}", skybox_faces[i]);
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // create frame buffer
    uint32_t fbo; 
    glGenFramebuffers(1, &fbo); 

    //It is also possible to bind a framebuffer to a read or write target specifically 
    // by binding to GL_READ_FRAMEBUFFER or GL_DRAW_FRAMEBUFFER respectively.
    glBindFramebuffer(GL_FRAMEBUFFER, fbo); 

    // an attachment is a memory location that can act as a buffer for the 
    // framebuffer, think of it as an image.
    // two options; texture or renderbuffer object

    // Create a texture attachment.
    uint32_t framebuffer_texture; 
    glGenTextures(1, &framebuffer_texture); 
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, kWidth, kHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_texture, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    // create render buffer object:  can not be directly read from.
    // store all render data directly in buffer without conversions to texture-specificc formats
    // faster as writeable storage, but can not directly read from them
    // often used as depth and stencil attachments
    uint32_t rbo; 
    glGenRenderbuffers(1, &rbo); 
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, kWidth, kHeight);

    //attach render buffer object to framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // the general rule is that if you never need to sample data from a specific buffer, 
    // it is wise to use a renderbuffer object for that specific buffer.If you need to sample data 
    // from a specific buffer like colors or depth values, you should use a texture attachment

    //For a framebuffer to be complete the following requirements have to be satisfied :
    // 1. We have to attach at least one buffer(color, depth or stencil buffer).
    // 2. There should be at least one color attachment.
    // 3. All attachments should be complete as well(reserved memory).
    // 4. Each buffer should have the same number of samples.
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        TY_CORE_ERROR("FRAMEBUFFER: Framebuffer is not complete");
    }
    //Bind the defualt framebuffer for rendering operations ot have a visual impact
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Render loop 
    while (!glfwWindowShouldClose(window))
    {
        // update time delta
        dt.Step();

        // input
        ProcessInput(window, dt.delta_time());

        // FIRST PASSS
        // ----------------------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, fbo); // Bind ccustom framebuffer

        // render commands 
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        glClearColor(0.1f, 0.1f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Scene
        // ----------------------------------------
        shader.Use();

        // face culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CCW);

        //camera matrice uniforms
        shader.SetMat4("view", camera.GetViewMatrix());
        shader.SetMat4("projection", camera.GetProjectionMatrix());
        shader.SetVec3("camera_pos", camera.GetPosition());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_id);

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

        // draw skybox as last
        // skybox
        glDisable(GL_CULL_FACE);
        skybox_shader.Use();
        skybox_shader.SetMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
        skybox_shader.SetMat4("projection", camera.GetProjectionMatrix());
        
        glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
        glBindVertexArray(skybox_vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_id);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);// set depth function back to default

        // SECOND PASS
        // ----------------------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   // back to default
        glClearColor(0.1f, 0.1f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        framebuffer_shader.Use(); 
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, framebuffer_texture); 
        glBindVertexArray(quad_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // check and call events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window); 
    }
    // delete textures
    // delete shader 
    // delete meshes

    
    // do not forget to delete framebuffer 
    glDeleteFramebuffers(1, &fbo); 

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
