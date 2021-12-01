#version 400

layout(location=0) in vec4 vp;
layout(location=1) in vec4 np;
layout(location=2) in vec2 texcoord;
out vec3 position;
out vec3 normal;
out vec2 uv_coords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {
  
  gl_Position = projection * view * model * vp;
  //�r detta det man ska g�ra p� Transformations to rasteriser vs fragment shader? 
  position = vec3(view * model * vp);
  normal = vec3(view * model * np);
  uv_coords = texcoord;

};