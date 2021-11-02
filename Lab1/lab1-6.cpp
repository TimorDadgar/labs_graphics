// GL Extension Wrangler
#include <GL/glew.h>
#include <cmath>
// http://www.glfw.org/faq.html#what-is-glfw
//OpenGL doesn't have a way of creating window, this does it in an OS-independent way
#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"

//-------------------------------------------------------------------------//
// You can store the rotation angles here, for example
float g_rotation[2] = { 0, 0 };
float far = -100.0;
float near = -1.0;
float right = 1.0;
float left = -1.0;
float top = 1.0;
float bottom = -1.0;
//-------------------------------------------------------------------------//
GLuint shader_program;
GLuint location_MVP;

void MUL_4x4 (GLfloat (*C)[4], const GLfloat (*A)[4], const GLfloat (*B)[4])
{
	//-------------------------------------------------------------------------//
	// YOUR CODE GOES HERE
	// Compute C = A x B
	
	// Courtesy of https://www.javatpoint.com/matrix-multiplication-in-c we modified the algorithm from them so it works here
	int r, c, i, j, k;
	r = 4; // rows
	c = 4; // columns

	// printf("multiply of the matrix=\n");
	for (i = 0; i < r; i++)
	{
		for (j = 0; j < c; j++)
		{
			for (k = 0; k < c; k++)
			{
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	//for printing result    
	/*
	for (i = 0; i < r; i++)
	{
		for (j = 0; j < c; j++)
		{
			printf("%f\t", C[i][j]);
		}
		printf("\n");
	}
	*/
	//-------------------------------------------------------------------------//
}

void invertMatrix (float (*C)[4], const float (*A)[4])
{
	// Compute C = A^(-1) for a transformation matrix.

	// Optional, only if you want to play with a more exciting view
	// transform than a simple translation.
  
	// (The rotation part can be inverted separately from the
	// translation part and the last row is the same.)
}


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
		//implement reloading of the shaders on the fly
		std::string vertex_shader_str = readFile("../lab1-6_vs.glsl");
		std::string fragment_shader_str = readFile("../lab1-6_fs.glsl");
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
		location_MVP = glGetUniformLocation(shader_program, "u_MVP");

	} 

	//-------------------------------------------------------------------------//
	// YOUR CODE GOES HERE
	// Update rotation angle here, for example
	//-------------------------------------------------------------------------//

	if ((key == GLFW_KEY_LEFT) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation[0] -= 0.17;
	}
	if ((key == GLFW_KEY_RIGHT) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation[0] += 0.17;
	}
	if ((key == GLFW_KEY_UP) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation[1] += 0.17;
	}
	if ((key == GLFW_KEY_DOWN) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation[1] -= 0.17;
	}
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
  
	GLFWwindow* window = glfwCreateWindow (640, 480, "Hello Cube", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
	int w_height = 600;
	int w_width = 800;

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent (window);
                                  
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit ();

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"


	//-------------------------------------------------------------------------//
	// COPY FROM lab1-5
	// Geometry, VBO, EBO, VAO
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

	// load and compile shaders  "../lab1-6_vs.glsl" and "../lab1-6_fs.glsl"
	
	std::string vertex_shader_str = readFile("../lab1-6_vs.glsl");
	std::string fragment_shader_str = readFile("../lab1-6_fs.glsl");
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
	shader_program = glCreateProgram();
	//-------------------------------------------------------------------------//
	// attach and link vertex and fragment shaders into a shader program
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(shader_program);
	location_MVP = glGetUniformLocation(shader_program, "u_MVP");
	//-------------------------------------------------------------------------//

	while (!glfwWindowShouldClose(window))
	{

		glfwGetFramebufferSize(window, &w_width, &w_height); //you might need this for correcting the aspect ratio

		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE
		// Define the projection matrix, rotation matrices, model matrix, etc.

		GLfloat rotate_x[4][4] = {
			{ 1, 0, 0, 0 },
			{ 0, cos(g_rotation[0]), -sin(g_rotation[0]), 0 },
			{ 0, sin(g_rotation[0]), cos(g_rotation[0]), 0 },
			{ 0, 0, 0, 1 }
		};
		GLfloat rotate_y[4][4] = { 
			{ cos(g_rotation[1]), 0, sin(g_rotation[1]), 0 },
			{ 0, 1, 0, 0 },
			{ -sin(g_rotation[1]), 0, cos(g_rotation[1]), 0 },
			{ 0, 0, 0, 1 }
		};
		GLfloat modelMatrix[4][4] = {
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 }
		};
		MUL_4x4(modelMatrix, rotate_x, rotate_y);
    
		GLfloat viewMatrix[4][4] = {
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 2 },
			{ 0, 0, 0, 1 }
		};
		GLfloat inverseViewMatrix[4][4] = {
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, -2 },
			{ 0, 0, 0, 1 }
		};
		//invertMatrix(inverseViewMatrix, viewMatrix);

		GLfloat modelViewMatrix[4][4] = {
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 }
		};
		MUL_4x4(modelViewMatrix, inverseViewMatrix, modelMatrix);
		GLfloat projectionMatrix[4][4] = {
			{ 2.0f / (right - left), 0.0f, 0.0f, -((right + left) / (right - left))},
			{ 0.0f, 2.0f / (top - bottom), 0.0f, -((top + bottom) / (top - bottom))},
			{ 0.0f, 0.0f, -(2.0f / (far - near)), -((far + near) / (far - near))},
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};
		GLfloat modelViewProjectionMatrix[4][4] = {
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 }
		};
		MUL_4x4(modelViewProjectionMatrix, projectionMatrix, modelViewMatrix);
    
    
		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE
		// Send your modelViewProjection matrix to your vertex shader as a uniform
		glUniformMatrix4fv(location_MVP, 1, GL_TRUE, *modelViewProjectionMatrix);
		//-----------------------------------------------------------------------//
    
		// update other events like input handling 
		glfwPollEvents ();
    
		// clear the drawing surface
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//-----------------------------------------------------------------------//
		// Issue an appropriate glDraw*() command.
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		//-----------------------------------------------------------------------//

		glfwSwapBuffers (window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

