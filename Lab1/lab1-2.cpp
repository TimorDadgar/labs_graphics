
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>  
#include <iostream>

static void error_callback(int error, const char* description)
{
	std::cerr << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
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
  
	GLFWwindow* window = glfwCreateWindow (640, 480,
	                                       "Hello Triangle", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent (window);
  
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit ();
	glClearColor(0.25f, 0.25f, 0.50f, 1.0f);


	//-------------------------------------------------------------------------//
	// YOUR CODE GOES HERE.
	// 1. Create geometry
	float points[3][3] = {
		{-1.0f, -1.0f, 1.1f},	//Left triangle
		{1.0f, -1.0f, -0.5f},	//Right triangle
		{1.0f, 1.0f, -0.5f}		//Top triangle
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
	//-------------------------------------------------------------------------//
  
	const char* vertex_shader =
		"#version 400\n"
		"in vec3 vp;"
		"void main () {"
		"  gl_Position = vec4 (vp, 1.0);"
		"}";
  
	const char* fragment_shader =
		"#version 400\n"
		"out vec3 frag_colour;"
		"void main () {"
		"  frag_colour = vec3 (0.5, 0.0, 0.5);"
		"}";

	GLuint vs = glCreateShader (GL_VERTEX_SHADER);
	glShaderSource (vs, 1, &vertex_shader, NULL);
	glCompileShader (vs);
	
	GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fs, 1, &fragment_shader, NULL);
	glCompileShader (fs);

	GLuint shader_programme = glCreateProgram ();
	glAttachShader (shader_programme, fs);
	glAttachShader (shader_programme, vs);
	glLinkProgram (shader_programme);

	glDeleteShader(vs);
	glDeleteShader(fs);

	glUseProgram (shader_programme);

	while (!glfwWindowShouldClose (window)) 
	{
		// update other events like input handling 
		glfwPollEvents ();
		// clear the drawing surface
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
		//-----------------------------------------------------------------------//
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//-----------------------------------------------------------------------//

		//refresh the displayed image
		glfwSwapBuffers (window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
