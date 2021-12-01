
bool wireframe = false;

// if ((key == GLFW_KEY_W ) && action == GLFW_PRESS)
// 	wireframe = !wireframe;



int main(int argc, char const *argv[])
{

	if(wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}
