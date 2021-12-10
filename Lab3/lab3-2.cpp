// Replace with your own code, starting from what you had in Lab 1,
// but use the geometry defined below.


#include <GL/glew.h>
#include <cmath>

#include <GLFW/glfw3.h>
#include <cstdlib>  
#include <iostream>
#include "readfile.hpp"

#include "lodepng.h"

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
		std::string vertex_shader_str = readFile("../lab3-2_vs.glsl");
		std::string fragment_shader_str = readFile("../lab3-2_fs.glsl");
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
	glm::vec3 p1, p2, p3, p4, p5;

	for (i = 0; i < len_f / 3; i++)
	{
		int p1_start = f[3 * i];
		int p2_start = f[3 * i + 1];
		int p3_start = f[3 * i + 2];

		p1 = glm::vec3(p[p1_start * 3], p[p1_start * 3 + 1], p[p1_start * 3 + 2]);
		p2 = glm::vec3(p[p2_start * 3], p[p2_start * 3 + 1], p[p2_start * 3 + 2]);
		p3 = glm::vec3(p[p3_start * 3], p[p3_start * 3 + 1], p[p3_start * 3 + 2]);

		glm::vec3 x = p2 - p1;
		glm::vec3 y = p3 - p1;
		glm::vec3 n_temp = glm::cross(x, y);

		n[p1_start] = glm::vec4(n_temp, 0);
		n[p2_start] = glm::vec4(n_temp, 0);
		n[p3_start] = glm::vec4(n_temp, 0);

	}

	for (int m = 0; m < 60; m++) {
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

	float t = (1.0f + sqrtf(5.0f)) * 0.25f;
	int num_faces = 20;
	int num_verts = 60;

	float points[] = {
		// An icosahedron without shared vertices has 20*3 vertices
		-0.5,  t,  0,
		-0.5,  t,  0,
		-0.5,  t,  0,
		-0.5,  t,  0,
		-0.5,  t,  0,

		0.5,  t,  0,
		0.5,  t,  0,
		0.5,  t,  0,
		0.5,  t,  0,
		0.5,  t,  0,

		-0.5, -t,  0,
		-0.5, -t,  0,
		-0.5, -t,  0,
		-0.5, -t,  0,
		-0.5, -t,  0,

		0.5, -t,  0,
		0.5, -t,  0,
		0.5, -t,  0,
		0.5, -t,  0,
		0.5, -t,  0,

		0, -0.5,  t,
		0, -0.5,  t,
		0, -0.5,  t,
		0, -0.5,  t,
		0, -0.5,  t,

		0,  0.5,  t,
		0,  0.5,  t,
		0,  0.5,  t,
		0,  0.5,  t,
		0,  0.5,  t,

		0, -0.5, -t,
		0, -0.5, -t,
		0, -0.5, -t,
		0, -0.5, -t,
		0, -0.5, -t,

		0,  0.5, -t,
		0,  0.5, -t,
		0,  0.5, -t,
		0,  0.5, -t,
		0,  0.5, -t,

		t,  0, -0.5,
		t,  0, -0.5,
		t,  0, -0.5,
		t,  0, -0.5,
		t,  0, -0.5,

		t,  0,  0.5,
		t,  0,  0.5,
		t,  0,  0.5,
		t,  0,  0.5,
		t,  0,  0.5,

		-t,  0, -0.5,
		-t,  0, -0.5,
		-t,  0, -0.5,
		-t,  0, -0.5,
		-t,  0, -0.5,

		-t,  0,  0.5,
		-t,  0,  0.5,
		-t,  0,  0.5,
		-t,  0,  0.5,
		-t,  0,  0.5
	};

	float uv_coords[] =
	{
	  0.072405, 0.144686,
	  0.27133,  0.10033,
	  0.481007, 0.129901,
	  0.681948, 0.115116,
	  0.928587, 0.107723,
	  0.29888,  0.70651,
	  0.449421, 0.736084,
	  0.108695, 0.129901,
	  0.847270, 0.699121,
	  0.868776, 0.728691,
	  0.576437, 0.721299,
	  0.319044, 0.750869,
	  0.548883, 0.736084,
	  0.388265, 0.115116,
	  0.397674, 0.713906,
	  0.149690, 0.122508,
	  0.347942, 0.115116,
	  0.583157, 0.107723,
	  0.780738, 0.100331,
	  0.229663, 0.107723,
	  0.33436,  0.68882,
	  0.181276, 0.721299,
	  0.379528, 0.721299,
	  0.190684, 0.122508,
	  0.358023, 0.736084,
	  0.103319, 0.713906,
	  0.24512,  0.70651,
	  0.083157, 0.699121,
	  0.31006,  0.10251,
	  0.222942, 0.721299,
	  0.772001, 0.728691,
	  0.618776, 0.721299,
	  0.745120, 0.721299,
	  0.433292, 0.115116,
	  0.598614, 0.713906,
	  0.520657, 0.721299,
	  0.650362, 0.736084,
	  0.700765, 0.728691,
	  0.878184, 0.107723,
	  0.671867, 0.743476,
	  0.914475, 0.743476,
	  0.819717, 0.736084,
	  0.201437, 0.721299,
	  0.633561, 0.122508,
	  0.796195, 0.743476,
	  0.140953, 0.728691,
	  0.122136, 0.706514,
	  0.262593, 0.721299,
	  0.157754, 0.728691,
	  0.825765, 0.115116,
	  0.723614, 0.728691,
	  0.897001, 0.721299,
	  0.499152, 0.736084,
	  0.731007, 0.115116,
	  0.469582, 0.721299,
	  0.045523, 0.691729,
	  0.967566, 0.721299,
	  0.28341,  0.67157,
	  0.532754, 0.122508,
	  0.418507, 0.706514,
	};

	int num_indices = num_faces * 3;
	unsigned short faces[] = {
		// ... and 20 triangular faces, 
		// defined by the following vertex indices:
		0,  55, 25,
		1,  26,  5,
		2,  6, 35,
		3,  36, 50,
		4,  51, 56,
		7,  27, 45,
		28, 57, 20,
		58, 52, 10,
		53, 37, 30,
		38,  8, 40,
		15, 46, 21,
		16, 22, 11,
		17, 12, 31,
		18, 32, 41,
		19, 42, 47,
		23, 48, 29,
		13, 24, 59,
		33, 14, 54,
		43, 34, 39,
		49, 44, 9
	};

	//Creating norm of each point
	glm::vec4 norm_vec4[60];
	normCalculator(points, faces, norm_vec4, sizeof(faces) / sizeof(faces[0])); //sizeof is divided by 2 since unsigned in is 2 bytes


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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	GLuint vbo_n;
	glGenBuffers(1, &vbo_n);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
	glBufferData(GL_ARRAY_BUFFER, sizeof(norm_vec4), &norm_vec4, GL_STATIC_DRAW); //Chose GL_STATIC_DRAW because it is accessing data once and then only used for drawing
	glEnableVertexAttribArray(1);
	// OBS! kolla ifall vec4 iterering går till såhär, med 3 som stride.
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	GLuint vbo_uv;
	glGenBuffers(1, &vbo_uv);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_coords), &uv_coords, GL_STATIC_DRAW); //Chose GL_STATIC_DRAW because it is accessing data once and then only used for drawing
	glEnableVertexAttribArray(2);
	// OBS! kolla ifall vec4 iterering går till såhär, med 3 som stride.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);


	// 3. Create EBO
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), &faces, GL_STATIC_DRAW);
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// load and compile shaders  "../lab3_vs.glsl" and "../lab3-1_fs.glsl"

	std::string vertex_shader_str = readFile("../lab3-2_vs.glsl");
	std::string fragment_shader_str = readFile("../lab3-2_fs.glsl");
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

	//creating texture handler
	GLuint texture_handle;
	glGenTextures(1, &texture_handle);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader_program, "tex_sampler"), 1);
	glBindTexture(GL_TEXTURE_2D, texture_handle);
	
	GLuint environment_handle;
	glGenTextures(1, &environment_handle);
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(shader_program, "env_sampler"), 1); // maybe 1 instead of 0
	glBindTexture(GL_TEXTURE_CUBE_MAP, environment_handle);
	
	//loading textures
	unsigned char* image_data;
	unsigned image_w;
	unsigned image_h;
	unsigned err_code =
		lodepng_decode32_file(&image_data, &image_w, &image_h,
			"../../common/data/numberline_hires.png");
	if (!err_code)
		std::cout << "Read " << image_h << " x " << image_w << " image\n";

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	//binding GL_TEXTURE_2D
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int i = 0;
	for ( i ; i < 6; i++ ) {
		switch (i) {
			case 0:
				err_code =
					lodepng_decode32_file(&image_data, &image_w, &image_h,
						"../../common/data/cube_right.png");
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
				break;
			case 1:
				err_code =
					lodepng_decode32_file(&image_data, &image_w, &image_h,
						"../../common/data/cube_left.png");
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
				break;
			case 2:
				err_code =
					lodepng_decode32_file(&image_data, &image_w, &image_h,
						"../../common/data/cube_up.png");
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
				break;
			case 3:
				err_code =
					lodepng_decode32_file(&image_data, &image_w, &image_h,
						"../../common/data/cube_down.png");
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
				break;
			case 4:
				err_code =
					lodepng_decode32_file(&image_data, &image_w, &image_h,
						"../../common/data/cube_back.png");
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
				break;
			case 5:
				err_code =
					lodepng_decode32_file(&image_data, &image_w, &image_h,
						"../../common/data/cube_front.png");
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
				break;
		}
		if (!err_code)
			std::cout << "Read " << image_h << " x " << image_w << " image\n";
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &w_width, &w_height);

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

		GLuint location_light = glGetUniformLocation(shader_program, "light_count");
		GLuint location_light_pos = glGetUniformLocation(shader_program, "light_position");
		GLuint location_light_col = glGetUniformLocation(shader_program, "light_colour");

		int light_count = 1;
		glm::vec3 light_pos = glm::vec3(0, 0, 30);
		//glm::vec3 light_positions[4] = { glm::vec3(3, 3, 3), glm::vec3(3, 3, 3), glm::vec3(3, 3, 3), glm::vec3(3, 3, 3) };
		glm::vec3 light_col = glm::vec3(1, 1, 1);

		glUniform1i(location_light, light_count);
		glUniform3fv(location_light_pos, 1, glm::value_ptr(light_pos));
		glUniform3fv(location_light_col, 1, glm::value_ptr(light_col));

		// -----------------------------------------------------------------------//


		// update other events like input handling 
		glfwPollEvents();

		// clear the drawing surface
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//-----------------------------------------------------------------------//
		// Issue an appropriate glDraw*() command.
		glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_SHORT, 0);

		//-----------------------------------------------------------------------//

		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}