#version 400

layout(location=0) in vec4 vp;
layout(location=1) in vec4 np;
out vec3 position;
out vec3 normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {
  
  gl_Position = projection * view * model * vp;

  position = vec3(view * model * vp);
  normal = vec3(view * model * np);

};