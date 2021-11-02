#version 400
in vec3 vp;
out vec3 position;

void main () {
	gl_Position = vec4 (vp, 1.0);
	position = vp + 0.6;
 };
  
