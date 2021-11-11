// Replace with your own code, starting from what you had in Lab 1,
// but use the geometry defined below.


#include <GL/glew.h>
#include <cmath>

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
#include "glm/ext.hpp" // print vec4

//-------------------------------------------------------------------------//
// You can store the rotation angles here, for example
float g_rotation[2] = { 45, 45 };
float far = 100.0;
float near = 1.0;

int w_height = 600;
int w_width = 800;
//-------------------------------------------------------------------------//
GLuint shader_program;


void checkShaderCompileError(GLint shaderID)
{
	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
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
	// Update rotation angle here, for example
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if ((key == GLFW_KEY_R) && action == GLFW_PRESS)
	{
		//implement reloading of the shaders on the fly
		std::string vertex_shader_str = readFile("../lab2-1_vs.glsl");
		std::string fragment_shader_str = readFile("../lab2-1_fs.glsl");
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
	}

	//0.17 för radianer, 5.0 för grader
	if ((key == GLFW_KEY_LEFT) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation[0] -= 5.0;
	}
	if ((key == GLFW_KEY_RIGHT) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation[0] += 5.0;
	}
	if ((key == GLFW_KEY_UP) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation[1] += 5.0;
	}
	if ((key == GLFW_KEY_DOWN) && ((action == GLFW_PRESS) || (action == GLFW_REPEAT))) {
		g_rotation[1] -= 5.0;
	}
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void normCalculator(float* p, unsigned short* f, glm::vec4* n, int len_f) {
	int i, j;
	glm::vec3 p1, p2, p3;

	// goes 12 iterations with a stride of 3
	//total of 36 different i:s
	for (i = 0; i < len_f / 3; i++)
	{
		int p1_start = f[3 * i];
		int p2_start = f[3 * i + 1];
		int p3_start = f[3 * i + 2];

		p1 = glm::vec3(p[p1_start * 4], p[p1_start * 4 + 1], p[p1_start * 4 + 2]);
		p2 = glm::vec3(p[p2_start * 4], p[p2_start * 4 + 1], p[p2_start * 4 + 2]);
		p3 = glm::vec3(p[p3_start * 4], p[p3_start * 4 + 1], p[p3_start * 4 + 2]);

		glm::vec3 x = p2 - p1;
		glm::vec3 y = p3 - p1;
		glm::vec3 n_temp = glm::cross(x, y);

		n[p1_start] = glm::vec4(n_temp, 0);
		n[p2_start] = glm::vec4(n_temp, 0);
		n[p3_start] = glm::vec4(n_temp, 0);

	}

	for (int m = 0; m < 24; m++) {
		std::cout << glm::to_string(n[m]) << std::endl;
	}
}

int main(int argc, char const* argv[])
{
	// start GL context and O/S window using the GLFW helper library

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);


	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	GLFWwindow* window = glfwCreateWindow(w_width, w_height, "Hello Icosahedron", NULL, NULL);
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

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// Set up geometry, VBO, EBO, VAO
	// 1. Create geometry


	float points[] = {
		// A cube has 8 vertices, but now we have three copies of each one:
		-0.5, -0.5, -0.5, 1, //0 0
		-0.5, -0.5, -0.5, 1, //0 1
		-0.5, -0.5, -0.5, 1, //0 2
		//
		-0.5, -0.5,  0.5, 1, //1 3
		-0.5, -0.5,  0.5, 1, //1 4
		-0.5, -0.5,  0.5, 1, //1 5
		//
		-0.5,  0.5, -0.5, 1, //2 6
		-0.5,  0.5, -0.5, 1, //2 7
		-0.5,  0.5, -0.5, 1, //2 8
		//
		-0.5,  0.5,  0.5, 1, //3 9
		-0.5,  0.5,  0.5, 1, //3 10
		-0.5,  0.5,  0.5, 1, //3 11
		//
		0.5, -0.5, -0.5, 1, //4 12
		0.5, -0.5, -0.5, 1, //4 13
		0.5, -0.5, -0.5, 1, //4 14
		//
		0.5, -0.5,  0.5, 1, //5 15
		0.5, -0.5,  0.5, 1, //5 16
		0.5, -0.5,  0.5, 1, //5 17
		//
		0.5,  0.5, -0.5, 1, //6 18
		0.5,  0.5, -0.5, 1, //6 19
		0.5,  0.5, -0.5, 1, //6 20
		//
		0.5,  0.5,  0.5, 1, //7 21
		0.5,  0.5,  0.5, 1, //7 22
		0.5,  0.5,  0.5, 1, //7 23
	};


	unsigned short faces[] = {
		// ... and 12 triangular faces, 
		// defined by the following vertex indices:
		0, 9, 6, // 0 3 2
		0, 3, 9, // 0 1 3
		//
		1, 7, 18, // 0 2 6
		1, 18, 12, // 0 6 4
		//
		13, 19, 15, // 4 6 5
		15, 19, 21, // 5 6 7
		//
		16, 22, 10, // 5 7 3
		16, 10, 4, // 5 3 1
		//
		8, 11, 23, // 2 3 7
		8, 23, 20, // 2 7 6
		//
		2, 14, 5, // 0 4 1
		5, 14, 17 // 1 4 5
	};

	//Creating norm of each point
	glm::vec4 norm_vec4[24];
	normCalculator(points, faces, norm_vec4, sizeof(faces)/sizeof(faces[0])); //sizeof is divided by 2 since unsigned in is 2 bytes


	// 2. Create VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 3. Create VBO
	GLuint vbo_p;
	glGenBuffers(1, &vbo_p);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_p);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW); //Chose GL_STATIC_DRAW because it is accessing data once and then only used for drawing
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	GLuint vbo_n;
	glGenBuffers(1, &vbo_n);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
	glBufferData(GL_ARRAY_BUFFER, sizeof(norm_vec4), &norm_vec4, GL_STATIC_DRAW); //Chose GL_STATIC_DRAW because it is accessing data once and then only used for drawing
	glEnableVertexAttribArray(1);
	// OBS! kolla ifall vec4 iterering går till såhär, med 4 som stride.
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);


	// 3. Create EBO
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), &faces, GL_STATIC_DRAW);
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// load and compile shaders  "../lab2_vs.glsl" and "../lab2-1_fs.glsl"

	std::string vertex_shader_str = readFile("../lab2-2_vs.glsl");
	std::string fragment_shader_str = readFile("../lab2-1_fs.glsl");
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

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	shader_program = glCreateProgram();
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// attach and link vertex and fragment shaders into a shader program
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	glAttachShader(shader_program, fs);
	glAttachShader(shader_program, vs);
	glLinkProgram(shader_program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glUseProgram(shader_program);

	while (!glfwWindowShouldClose(window))
	{


		glfwGetFramebufferSize(window, &w_width, &w_height);

		//-----------------------------------------------------------------------//
		// YOUR CODE GOES HERE
		//
		// Use glm::perspective to create a projection matrix
		//
		// Use glm::translate, glm::rotate and glm::inverse to create the
		// model and view matrices.

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::mat4 viewMatrix = glm::mat4(1.0f);
		glm::mat4 projectionMatrix = glm::mat4(1.0f);

		modelMatrix = glm::rotate(modelMatrix, glm::radians(g_rotation[0]), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(g_rotation[1]), glm::vec3(1.0f, 0.0f, 0.0f));

		viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -2.0f));
		projectionMatrix = glm::perspective(glm::radians(60.0f), float(w_width) / float(w_height), near, far);

		GLuint location_model = glGetUniformLocation(shader_program, "model");
		GLuint location_view = glGetUniformLocation(shader_program, "view");
		GLuint location_projection = glGetUniformLocation(shader_program, "projection");


		// uniform variable in vertex shader.
		glUniformMatrix4fv(location_model, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(location_view, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(location_projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		// -----------------------------------------------------------------------//


		// update other events like input handling 
		glfwPollEvents();

		// clear the drawing surface
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//-----------------------------------------------------------------------//
		// Issue an appropriate glDraw*() command.
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

		//-----------------------------------------------------------------------//

		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}