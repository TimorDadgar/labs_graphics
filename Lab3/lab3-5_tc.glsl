// Tesselation Control Shader
#version 440

layout(vertices = 3) out;

// User-defined inputs from vertex shader
in vec3 vPosition[];
in vec2 vTexCoord[];

// User-defined outputs to Tessellation Evaluation Shader
out vec3 tcPosition[];
out vec2 tcTexCoord[];

// Try different levels here!
float TessLevelInner = 64;
float TessLevelOuter = 1;


void main()
{
  // In the Tesselation ControlShader gl_InvocationID is the
  // invocation (0,1,2,3) of the shader since it is called once per
  // vertex in the patch.
  
  // Just pass through positions and texture coordinates
  tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];
  tcTexCoord[gl_InvocationID] = vTexCoord[gl_InvocationID];

  // Set tessellation levels. We only need to do this for one of the
  // invocations, since the levels must be the same for the whole
  // patch.
  if (gl_InvocationID == 0) 
  {
    gl_TessLevelInner[0] = TessLevelInner;
    gl_TessLevelOuter[0] = TessLevelOuter;
    gl_TessLevelOuter[1] = TessLevelOuter;
    gl_TessLevelOuter[2] = TessLevelOuter;
  }
}
