#version 400

layout(location=0) in vec4 vp;
uniform mat4 u_MVP;

void main () {
	
	//-------------------------------------------------------------------------//
	// Instead of passing the `vp` variable unaffected as below, 
	// apply the model, view and projection transform to vertex
	// positions here. Forward the position to the fragment shader using
	// an appropriate `out` variable.
	// -------------------------------------------------------------------------//

	gl_Position = u_MVP * vp;

};
  
