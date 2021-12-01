// Vertex Shader
#version 440

layout(location=0) in vec4 Position;
layout(location=1) in vec2 TexCoord;

out vec3 vPosition;
out vec2 vTexCoord;

void main () {

  vTexcoord = TexCoord;
  vPosition = Position.xyz;

  // Note: not using the uniform Model/View/ProjectionMatrix here!
  
};
  
