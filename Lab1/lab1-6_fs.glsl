#version 400
out vec3 frag_colour;

void main () {

	// Set color proportional to the z position here, instead of a constant. 
	frag_colour = vec3(0.5);
}
