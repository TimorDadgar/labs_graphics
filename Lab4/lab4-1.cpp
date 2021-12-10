#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>

#include "readfile.hpp"

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> //glm::value_ptr

#ifndef M_PI
#define M_PI 3.14159265359
#endif


glm::vec4 mouse_state = glm::vec4(0);
glm::vec3 position, dir, right, up = glm::vec3(0);  // Camera parameters

GLuint textures[2];

float delta_time= 0.0f;
float yaw = M_PI;
float pitch = 0;
float focal_dist = 512.0f;
float move_speed = 0.08; 
float mouse_speed = 0.001f;
float brightness = 15;

unsigned int frame_count = 0;
int w_width = 512;
int w_height = 512;
bool first_frame = true;

void checkShaderCompileError(GLint shaderID)
{
  GLint isCompiled = 0;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
 
  if(isCompiled == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::string errorLog;
    errorLog.resize(maxLength);
    glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

    std::cout << "shader compilation failed:" << std::endl;
    std::cout << errorLog << std::endl;
    return;
  }
  else
    std::cout << "shader compilation success." << std::endl;
    
  return;
}


static void error_callback(int error, const char* description)
{
    std::cerr << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  frame_count = 0;
  // Move forward
  if ( (key == GLFW_KEY_UP   || key == GLFW_KEY_W) && 
       (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    position += dir * move_speed;
  }
  // Move backward
  if ( (key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && 
       (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    position -= dir * move_speed;
  }
  // Strafe right
  if ( (key == GLFW_KEY_RIGHT|| key == GLFW_KEY_D) && 
       (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    position += right * move_speed;
  }
  // Strafe left
  if ( (key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && 
       (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
    position -= right * move_speed;
  }
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS )
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void update_camera_basis()
{
  dir = glm::vec3(cos(pitch) * sin(yaw), 
                  sin(pitch),
                  cos(pitch) * cos(yaw));

  right = glm::vec3(  sin(yaw - M_PI/2.0f), 
                                0,
                                cos(yaw - M_PI/2.0f));
  up = glm::cross( right, dir );
}

static void scroll_callback(GLFWwindow* window, double scroll_v, double scroll_h)
{
  mouse_state.x += scroll_v;
  mouse_state.y += scroll_h;
  brightness += scroll_h;
  frame_count = 0;
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

  frame_count = 0;
  //only happens once
  static double previous_time = glfwGetTime();
  double current_time = glfwGetTime();
  delta_time = static_cast<float>(current_time - previous_time);

  // Reset mouse position
  glfwSetCursorPos(window, w_width/2.0f, w_height/2.0f);

  if(first_frame){  
    update_camera_basis();
    first_frame = false; 
    yaw = M_PI;
    pitch = 0;

    return;
  }

  // Compute new orientation
  yaw   += mouse_speed * float( w_width/2.0f - xpos );
  pitch += mouse_speed * float( w_height/2.0f - ypos );
  update_camera_basis();
    
  previous_time = current_time;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    mouse_state.z = 1;

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    mouse_state.w = 1;

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    mouse_state.z = 0;

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    mouse_state.w = 0;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_width, w_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_width, w_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    focal_dist = (width + height)*0.5;

}


int main(int argc, char const *argv[])
{
  // start GL context and O/S window using the GLFW helper library
  
  glfwSetErrorCallback(error_callback);
  if( !glfwInit() )
    exit(EXIT_FAILURE);
  
  GLFWwindow* window = glfwCreateWindow (w_width, w_height, "Hello Raycasting", NULL, NULL);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetScrollCallback ( window, scroll_callback); 

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  glDisable(GL_DEPTH_TEST);


  if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent (window);
                                  
  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit ();

  //this will be the image plane: two triangles, spanning the entire screen.
  float points[] = {
    1, 1, 0,    //Triangle 1
    1, -1, 0,
    -1, 1, 0, 

    -1, -1, 0,  //Triangle 2
    -1, 1, 0,
    1, -1, 0
  };

  GLuint vbo = 0;
  glGenBuffers (1, &vbo);
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glBufferData (GL_ARRAY_BUFFER, sizeof (points), points, GL_STATIC_DRAW);
  
  GLuint vao = 0;
  glGenVertexArrays (1, &vao);
  glBindVertexArray (vao);
  glEnableVertexAttribArray (0);
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  std::string vertex_shader_str = readFile("../lab4_vs.glsl");
  std::string fragment_shader_str = readFile("../lab4-1_fs.glsl");
  const char *vertex_shader_src = vertex_shader_str.c_str();
  const char *fragment_shader_src = fragment_shader_str.c_str();

  GLuint vs = glCreateShader (GL_VERTEX_SHADER);
  glShaderSource (vs, 1, &vertex_shader_src, NULL);
  glCompileShader (vs);
  checkShaderCompileError(vs);

  GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
  glShaderSource (fs, 1, &fragment_shader_src, NULL);
  glCompileShader (fs);
  checkShaderCompileError(fs);

  GLuint shader_program = glCreateProgram ();
  glAttachShader (shader_program, fs);
  glAttachShader (shader_program, vs);
  glLinkProgram (shader_program);
  
  glDeleteShader(vs);
  glDeleteShader(fs);

  float start_time = glfwGetTime();

  glBindVertexArray (vao);
  update_camera_basis();
  
  
  glGenTextures(2, textures);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_width, w_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glUniform1i(glGetUniformLocation(shader_program, "i_texture"), 0);

  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_width, w_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  GLuint fb;
  glGenFramebuffers(1, &fb);
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);
  glUseProgram (shader_program);
  
  //Does the GPU support current FBO configuration?
  if( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      exit(EXIT_FAILURE);

  position = glm::vec3( 0.5, 0.5, 2.5);
  

  while (!glfwWindowShouldClose (window)) 
  {
    glfwGetFramebufferSize(window, &w_width , &w_height );
   
    // update other events like input handling 
    glfwPollEvents ();

    glUniform1f(glGetUniformLocation(shader_program, "i_global_time"),    glfwGetTime()-start_time);
    glUniform4f(glGetUniformLocation(shader_program, "i_mouse_state"),    mouse_state.x,  mouse_state.y, mouse_state.z, mouse_state.w);
    glUniform2f(glGetUniformLocation(shader_program, "i_window_size"),    w_width, w_height);
    glUniform1ui(glGetUniformLocation(shader_program,"i_frame_count"),    frame_count++);
    glUniform3f(glGetUniformLocation(shader_program, "i_position"),       position.x, position.y, position.z);
    glUniform3f(glGetUniformLocation(shader_program, "i_up"),             up.x,  up.y, up.z);
    glUniform3f(glGetUniformLocation(shader_program, "i_dir"),            dir.x,  dir.y, dir.z);
    glUniform3f(glGetUniformLocation(shader_program, "i_right"),          right.x,  right.y, right.z);
    glUniform1f(glGetUniformLocation(shader_program, "i_focal_dist"),     focal_dist);
    glUniform3f(glGetUniformLocation(shader_program, "i_light_position"), 6e3,  1e4, 1e4);
    glUniform3f(glGetUniformLocation(shader_program, "i_light_color"), brightness, brightness, brightness);

    
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[0], 0);
    
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glActiveTexture(GL_TEXTURE0);

    double tic = glfwGetTime();
    glUniform1i(glGetUniformLocation(shader_program, "i_display"), 0);
    glDrawArrays(GL_TRIANGLES,0, sizeof(points)/sizeof(float)/3); // points is a float-array, with 3 verts/triangle
    
    double tic_toc = glfwGetTime() - tic;
    if(tic_toc > 1.0) continue;
    
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    glUniform1i(glGetUniformLocation(shader_program, "i_display"), 1);
    glDrawArrays(GL_TRIANGLES,0, sizeof(points)/sizeof(float)/3);
    
    glfwSwapBuffers (window);
  
    GLuint temp = textures[0];
    textures[0] = textures[1];
    textures[1] = temp;
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
