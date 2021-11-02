#version 400
out vec3 frag_colour;
uniform float modifier;
in vec3 position;

void main () {

	// Change this to use input from a `uniform` variable instead of just a constant color:
	frag_colour = position * modifier;
}
