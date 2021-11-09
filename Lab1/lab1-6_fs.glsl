#version 400
in vec3 position;
out vec3 frag_colour;

void main () {

	// Set color proportional to the z position here, instead of a constant. 
	frag_colour = vec3(0.5, 0.8, 0.5 + position.z);
}
