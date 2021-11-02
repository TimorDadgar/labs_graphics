
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"

GLuint location_pos; 
GLuint location_color;
GLfloat pos[2] = { 0.0, 0.0 };
GLfloat color_modifier = 1;
GLuint shader_program;

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
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
    
	if ((key == GLFW_KEY_R) && action == GLFW_PRESS) {
		//-----------------------------------------------------------------------//
		// COPY YOUR PREVIOUS SOLUTION FROM lab1-3.cpp HERE.
		// Reload shaders
		std::string vertex_shader_str = readFile("../lab1-3_vs.glsl");
		std::string fragment_shader_str = readFile("../lab1-3_fs.glsl");
		const char* vertex_shader_src = vertex_shader_str.c_str();
		const char* fragment_shader_src = fragment_shader_str.c_str();

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertex_shader_src, NULL);
		glCompileShader(vs);
		checkShaderCompileError(vs);

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragment_shader_src, NULL);
		glCompileShader(fs);
		checkShaderCompileError(fs);

		shader_program = glCreateProgram();
		glAttachShader(shader_program, fs);
		glAttachShader(shader_program, vs);
		glLinkProgram(shader_program);
		glDeleteShader(vs);
		glDeleteShader(fs);
		glUseProgram(shader_program);
		//-----------------------------------------------------------------------//
	} 

	//-----------------------------------------------------------------------//
	// YOUR CODE GOES HERE
	// Update some parameter for the vertex shader on arrow keys.
	//-----------------------------------------------------------------------//
	if ((key == GLFW_KEY_LEFT) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		pos[0] -= 0.1;
	}
	if ((key == GLFW_KEY_RIGHT) && ( (action == GLFW_PRESS) || (action == GLFW_REPEAT))){
		pos[0] += 0.1;
	}
	if ((key == GLFW_KEY_UP) && ( (action == GLFW_PRESS) || (action == GLFW_REPEAT))){
		pos[1] += 0.1;
	}
	if ((key == GLFW_KEY_DOWN) && ( (action == GLFW_PRESS) || (action == GLFW_REPEAT))){
		pos[1] -= 0.1;
	}
}

static void scroll_callback(GLFWwindow* window, double scroll_v, double scroll_h)
{
	//-----------------------------------------------------------------------//
	// YOUR CODE GOES HERE
	// Update some parameter for the fragment shader when scrolling.
	if (scroll_h == 1 && color_modifier < 5) {
		color_modifier += 0.1;
	}
	if(scroll_h == -1 && color_modifier >= 0.1){
		color_modifier -= 0.1;
	}
	//-----------------------------------------------------------------------//
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int main(int argc, char const *argv[])
{
	// start GL context and O/S window using the GLFW helper library
  
	glfwSetErrorCallback(error_callback);
	if( !glfwInit() )
		exit(EXIT_FAILURE);
  
	GLFWwindow* window = glfwCreateWindow (640, 480, "Hello Triangle", NULL, NULL);
	glfwSetKeyCallback( window, key_callback);
	glfwSetScrollCallback( window, scroll_callback); 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent (window);
                                  
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit ();

	//-------------------------------------------------------------------------//
	// 1. Create geometry
	float points[3][3] = {
		{-0.5f, -0.5f, 0.0f},	//Left triangle
		{0.5f, -0.5f, 0.0f},	//Right triangle
		{0.0f, 0.5f, 0.0f}	//Top triangle
	};

	// 2. Create VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 3. Create VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), &points, GL_STATIC_DRAW); //Chose GL_STATIC_DRAW because it is accessing data once and then only used for drawing
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//-------------------------------------------------------------------------//

	//-------------------------------------------------------------------------//
	// load and compile shaders  "../lab1-4_vs.glsl" and "../lab1-4_fs.glsl"
	// See corresponding code from the previous tasks.
	
	std::string vertex_shader_str = readFile("../lab1-4_vs.glsl");
	std::string fragment_shader_str = readFile("../lab1-4_fs.glsl");
	const char* vertex_shader_src = vertex_shader_str.c_str();
	const char* fragment_shader_src = fragment_shader_str.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader_src, NULL);
	glCompileShader(vs);
	checkShaderCompileError(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader_src, NULL);
	glCompileShader(fs);
	checkShaderCompileError(fs);

	shader_program = glCreateProgram();
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(shader_program);

	//-------------------------------------------------------------------------//

	//-------------------------------------------------------------------------//
	// attach and link vertex and fragment shaders into a shader program
	// See corresponding code from the previous tasks.
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(shader_program);
	location_pos = glGetUniformLocation(shader_program, "position_offset");
	location_color = glGetUniformLocation(shader_program, "modifier");
	//-------------------------------------------------------------------------//

	while (!glfwWindowShouldClose (window)) 
	{
		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE
		// Update uniform variables in your shader_program
		glUniform2f(location_pos, pos[0], pos[1]);
		glUniform1f(location_color, color_modifier);
		//-----------------------------------------------------------------------//

		// update other events like input handling 
		glfwPollEvents ();
		// clear the drawing surface
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//-----------------------------------------------------------------------//
		// COPY YOUR SOLUTION FROM lab1-2.cpp HERE.
		// Issue an appropriate glDraw*() command.
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//-----------------------------------------------------------------------//

		glfwSwapBuffers (window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

