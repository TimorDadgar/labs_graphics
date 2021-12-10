#version 400

layout(location=0) in vec4 vp;
layout(location=1) in vec4 np;
layout(location=2) in vec2 texcoord;
layout(location=3) in vec4 t;
layout(location=4) in vec4 b;
out vec3 position;
out vec3 normal;
out vec2 uv_coords;
out vec4 tangent;
out vec4 bitangent;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {
  
  gl_Position = projection * view * model * vp;
  //Är detta det man ska göra på Transformations to rasteriser vs fragment shader? 
  position = vec3(view * model * vp);
  normal = vec3(view * model * np);
  uv_coords = texcoord;

  tangent = vec4(view * model * normalize(t));
  bitangent = vec4(view * model * normalize(b));

};