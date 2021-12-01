//Tesselation Evaluation Shader
#version 440

// We work with triangle patches
layout(triangles) in;

// Inputs from TCS
in vec3 tcPosition[];
in vec2 tcTexCoord[];

// Outputs, to Fragment Shader
out vec3 tePosition;
out vec3 teNormal;
out vec2 teTexCoord;

// This is our displacement map
uniform sampler2D heightmap;

// We'll finally apply these uniforms now 
uniform mat4 modelMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;


vec4 gaussian_sample(const sampler2D tex, const vec2 uv, const float radius, const vec2 t1, const vec2 t2)
{
  //sampling is done along the uv directions, with attention to scale,
  //for non-square textures.
  vec2 ts = vec2(length(t1), length(t2))*radius;
  
  return	(1.0/4.0)  * texture(tex,uv) +
    (1.0/8.0)  * texture(tex,uv + t1*ts.x) +
    (1.0/8.0)  * texture(tex,uv - t1*ts.x) +
    (1.0/8.0)  * texture(tex,uv + t2*ts.y) +
    (1.0/8.0)  * texture(tex,uv - t2*ts.y) +
    (1.0/16.0) * texture(tex,uv + t1*ts.x + t2*ts.y) +
    (1.0/16.0) * texture(tex,uv + t1*ts.x - t2*ts.y) +
    (1.0/16.0) * texture(tex,uv - t1*ts.x + t1*ts.y) +
    (1.0/16.0) * texture(tex,uv - t1*ts.x - t1*ts.y);
}

const float sample_offset = 0.02;
const float displacement_coef = 0.25;

void main()
{

  // 1. Compute interpolated Position and TexCoord using gl_TessCoord
  // with tcPosition and tcTexCoord, respectively.

	// vec3 v = ...;
	// vec2 uv = ...;
	
  // 2. Compute the normal for the triangle that connects tcPosition[]

	// vec3 n = ...;
	
  // 3. Compute the amount of displacement for the vertex `v` by using the
  // sampler2D heightmap

  // 4a. Select 2 other texture coordinates and compute the
  // displacement at those points.
  
  // 4b. Use the three displaced points to compute a new normal, and
  // put it in `n`

  // 5. Now apply transformations  

	gl_Position = modelViewProjectionMatrix * vec4(v, 1);
  tePosition = modelViewMatrix * vec4(v, 1);
  teNormal = modelViewMatrix * vec4(n, 0);

}
