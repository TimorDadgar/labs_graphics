// GL Extension Wrangler
//https://www.opengl.org/sdk/libs/GLEW/
//What does GLEW do for you? What would it look like if i did its job manually?
//https://www.opengl.org/wiki/Load_OpenGL_Functions
#include <GL/glew.h>

// http://www.glfw.org/faq.html#what-is-glfw
//OpenGL doesn't have a way of creating window, this does it in an
//OS-independent way
#include <GLFW/glfw3.h>

#include <cstdlib>  
#include <iostream>

static void error_callback(int error, const char* description)
{
	std::cerr << description;
}

int main(int argc, char const *argv[])
{
	// start GL context and O/S window using the GLFW helper library
  
	glfwSetErrorCallback(error_callback);
	if( !glfwInit() )
		exit(EXIT_FAILURE);
	GLFWwindow* window = glfwCreateWindow (20, 20, "Hello OpenGL", NULL, NULL);
  
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent (window);
                                  
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit ();
  

	//-------------------------------------------------------------------------//
	// YOUR CODE GOES HERE.
	// Query renderer and version using glGetString, and print out
	// instead of the "???" placeholders below.


	std::cout << "Renderer:" << std::endl << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported:" << std::endl << glGetString(GL_VERSION) << std::endl;
	//-------------------------------------------------------------------------//
  
	// close GL context and any other GLFW resources
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
