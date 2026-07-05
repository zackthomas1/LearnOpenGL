#include "pch.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "Model.h"
#include "cameras/fly_camera.h"
#include "cameras/orthographic_camera.h"
#include "delta_time.h"
#include "primatives/cube.h"
#include "primatives/plane.h"
#include "shader_s.h"
#include "texture_2d.h"

// Declare functions
void ProcessInput(GLFWwindow* window, float time_step);
void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos,
                                     double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* CreateGLFWWindow();
unsigned int loadTexture(char const* path, bool gammaCorrection);

// Declare window size constants
const unsigned int kWidth = 800;
const unsigned int kHeight = 600;

// app global parameters
float last_x = (float)kWidth / 2.0f, last_y = (float)kHeight / 2.0f;
bool first_mouse = true;
bool is_blinn = false;
bool is_gamma = false;

LearnOpenGL::FlyCamera camera;
LearnOpenGL::DeltaTime& dt = LearnOpenGL::DeltaTime::getInstance();

int main(void) {
  LearnOpenGL::Log::Init();
  TY_CORE_INFO("Initialize Logging");

  // Create window
  glfwWindowHint(GLFW_SAMPLES, 4);
  GLFWwindow* window = CreateGLFWWindow();
  if (window == nullptr) return -1;

  // Initialize models
  float planeVertices[] = {
      // positions            // normals         // texcoords
       10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
      -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f,
      -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f, 0.0f,  10.0f,

       10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
      -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f, 0.0f,  10.0f,
       10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f, 10.0f, 10.0f};
  uint32_t vao, vbo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(6 * sizeof(float)));
  glBindVertexArray(0);

  LearnOpenGL::Cube cube;

  // create shader programs
  LearnOpenGL::Shader blinn_shader("../assets/shaders/5_3_blinn_phong.vs",
                             "../assets/shaders/5_3_blinn_phong.fs");

  // load textures
  uint32_t floorTexture = loadTexture("../assets/textures/wood.png", false);
  uint32_t floorTexture_gamma_correct =
      loadTexture("../assets/textures/wood.png", true);

  blinn_shader.Use();
  blinn_shader.SetInt("floorTexture", 0);

  // lighting info
  // -------------
  blinn_shader.Use();
  blinn_shader.SetVec3("lightDir", glm::vec3(2.0f, 4.0f, 1.0f));
  blinn_shader.SetVec3("lightColor", glm::vec3(0.5f));

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // update time delta
    dt.Step();

    // input
    ProcessInput(window, dt.delta_time());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    blinn_shader.Use();
    blinn_shader.SetMat4("view", camera.GetViewMatrix());
    blinn_shader.SetMat4("projection", camera.GetProjectionMatrix());
    blinn_shader.SetVec3("viewPos", camera.GetPosition());
    blinn_shader.SetInt("isBlinn", is_blinn);
    blinn_shader.SetInt("isGamma", is_gamma);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,
                  is_gamma ? floorTexture_gamma_correct : floorTexture);
    
    blinn_shader.SetVec4("translate", glm::vec4(0.0f, 2.0f, 2.0f, 0.0f));
    cube.Draw();

    blinn_shader.SetVec4("translate", glm::vec4(2.0f, 2.0f, 0.0f, 0.0f));
    cube.Draw();

    blinn_shader.SetVec4("translate", glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    cube.Draw();

    // draw floor
    blinn_shader.SetVec4("translate", glm::vec4(0.0f));
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,
                  is_gamma ? floorTexture_gamma_correct : floorTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // check and call events and swap buffers
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  // Terminate window
  glfwTerminate();
  return 0;
}

void ProcessInput(GLFWwindow* window, float time_step) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    TY_CORE_TRACE("Key W Press");
    camera.ProcessKeyboard(LearnOpenGL::FORWARD, time_step);
  } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    TY_CORE_TRACE("Key S Press");
    camera.ProcessKeyboard(LearnOpenGL::BACKWARD, time_step);
  } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    TY_CORE_TRACE("Key A Press");
    camera.ProcessKeyboard(LearnOpenGL::LEFT, time_step);
  } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    TY_CORE_TRACE("Key D Press");
    camera.ProcessKeyboard(LearnOpenGL::RIGHT, time_step);
  } else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    TY_CORE_TRACE("Key Q Press");
    camera.ProcessKeyboard(LearnOpenGL::DOWN, time_step);
  } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    TY_CORE_TRACE("Key E Press");
    camera.ProcessKeyboard(LearnOpenGL::UP, time_step);
  }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mods) {
  if (key == static_cast<int>(GLFW_KEY_B) &&
      action == static_cast<int>(GLFW_PRESS)) {
    is_blinn = !is_blinn;
    is_blinn ? TY_CORE_INFO("Blinn-Phong") : TY_CORE_INFO("Phong");
  }
  if (key == static_cast<int>(GLFW_KEY_G) &&
      action == static_cast<int>(GLFW_PRESS)) {
    is_gamma = !is_gamma;
    is_gamma ? TY_CORE_INFO("gamma correction: ON")
             : TY_CORE_INFO("gamma correction: OFF");
  }
}

static void cursor_position_callback(GLFWwindow* window, double xpos,
                                     double ypos) {
  if (first_mouse)  // initially set to true
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

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
  camera.ProcessMouseScroll((float)y_offset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

GLFWwindow* CreateGLFWWindow() {
  // Initialize GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // Create GLFW Window Object
  GLFWwindow* window = glfwCreateWindow(kWidth, kHeight, "OpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(
      window,
      framebuffer_size_callback);  // set call-back function for window resize
  glfwSetKeyCallback(window, key_callback);  // set call-back function key input
  glfwSetCursorPosCallback(
      window,
      cursor_position_callback);  // set call-back function cursor pos input
  glfwSetScrollCallback(window, scroll_callback);

  glfwSetInputMode(window, GLFW_CURSOR,
                   GLFW_CURSOR_DISABLED);  // capture mouse cursor

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to intialize GLAD" << std::endl;
    return nullptr;
  }

  // Set rendering window size
  glViewport(0, 0, kWidth, kHeight);

  return window;
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path, bool gammaCorrection) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
  if (data) {
    GLenum internalFormat;
    GLenum dataFormat;
    if (nrComponents == 1) {
      internalFormat = dataFormat = GL_RED;
    } else if (nrComponents == 3) {
      internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
      dataFormat = GL_RGB;
    } else if (nrComponents == 4) {
      internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
      dataFormat = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}