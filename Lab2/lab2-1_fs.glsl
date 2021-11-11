// Build on your code from Lab 1 here
#version 400
in vec3 position;
in vec3 normal;
out vec3 frag_colour;

// vec3 hsv2rgb(vec3 c)
// {
//     vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
//     vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
//     return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
// }

void main () {
	frag_colour.r = (normal.x+1)*0.5;
	frag_colour.g = (normal.y+1)*0.5;
	frag_colour.b = (normal.z+1)*0.5;

	//frag_colour = vec3(position) + 0.45;
}

