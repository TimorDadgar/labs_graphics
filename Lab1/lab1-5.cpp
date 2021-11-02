
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"

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
    
	if ((key == GLFW_KEY_R) && action == GLFW_PRESS)
	{
		//-----------------------------------------------------------------------//
		// COPY YOUR PREVIOUS SOLUTION FROM lab1-3.cpp HERE.
		// Reload shaders
		std::string vertex_shader_str = readFile("../lab1-5_vs.glsl");
		std::string fragment_shader_str = readFile("../lab1-5_fs.glsl");
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
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char const* argv[])
{
	// start GL context and O/S window using the GLFW helper library

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Cube", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	//-------------------------------------------------------------------------//
	// YOUR CODE GOES HERE.
	// Create geometry and VBO and VAO, and also an EBO.
	// (The geometry will be new, so you cannot only copy your previous code.)
	// 1. Create geometry
	float points[] = {
		-0.5 , -0.5 , -0.5, // 0 vertix
		-0.5 , -0.5 , 0.5, // 1
		-0.5 ,0.5 , -0.5, // 2
		-0.5 ,0.5 , 0.5, // 3
		0.5 , -0.5 , -0.5, // 4
		0.5 , -0.5 , 0.5, // 5
		0.5 ,0.5 , -0.5, // 6
		0.5 ,0.5 , 0.5 // 7
	};

	unsigned short faces[] = {
		// ... and 12 triangular faces, defined by these vertex indices:
		0, 3, 2, // triangle 1
		0, 1, 3, // triangle 2
		1, 7, 3, // triangle 3
		1, 5, 7, // triangle 4
		5, 6, 7, // triangle 5
		5, 4, 6, // triangle 6
		4, 2, 6, // triangle 7
		4, 0, 2, // triangle 8
		2, 7, 6, // triangle 9
		2, 3, 7, // triangle 10
		1, 4, 0, // triangle 11
		1, 5, 4, // triangle 12
	};
	// 2. Create VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 3. Create VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW); //Chose GL_STATIC_DRAW because it is accessing data once and then only used for drawing
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);


	// 3. Create EBO
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), &faces, GL_STATIC_DRAW);
	
	//removing 
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//-------------------------------------------------------------------------//

	//-------------------------------------------------------------------------//
	// load and compile shaders  "../lab1-4_vs.glsl" and "../lab1-4_fs.glsl"
	// See corresponding code from the previous tasks.
	std::string vertex_shader_str = readFile("../lab1-5_vs.glsl");
	std::string fragment_shader_str = readFile("../lab1-5_fs.glsl");
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

	//-------------------------------------------------------------------------//
 
	GLuint shader_program = glCreateProgram ();
	//-------------------------------------------------------------------------//
	// attach and link vertex and fragment shaders into a shader program
	// See corresponding code from the previous tasks.
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(shader_program);
	//-------------------------------------------------------------------------//

 
	while (!glfwWindowShouldClose (window)) 
	{

		// update other events like input handling 
		glfwPollEvents ();
    
		// clear the drawing surface
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE.
		// Issue an appropriate glDraw*() command.
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		//-----------------------------------------------------------------------//
    
		glfwSwapBuffers (window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

