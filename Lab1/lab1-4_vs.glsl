#version 400
in vec3 vp;
uniform vec2 position_offset;
out vec3 position;

void main () {
  
	// Change this so that you don't just set `gl_Position` to `vp`, but
	// also add an offset from a `uniform` variable:
	gl_Position = vec4(vp.x + position_offset.x, vp.y + position_offset.y,1.0 , 1.0);
	position = vp + vec3(position_offset, 0) + 0.45;
};
  
