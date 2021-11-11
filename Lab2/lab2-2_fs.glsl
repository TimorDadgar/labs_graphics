#version 440

out vec3 frag_colour;
in vec3 position;
in vec3 normal;

uniform int light_count;
uniform vec3 light_position[4]; //add [4] // can have up to 4 light sources
uniform vec3 light_colour[4]; //add [4]

//vec4 lambertian_brdf( vec3 in_direction, vec3 out_direction, vec3 normal )
vec4 lambertian_brdf()
{
  // YOUR CODE GOES HERE
  // Implement a Lambertian BRDF 
  

    return (vec4(0.3,0.8,0.8,0));
}

void main () {

    vec3 light_direction = light_position[0] - position;
    for ( int l = 0; l < light_count; ++l )
    {
    // YOUR CODE GOES HERE
    // Implement Equation 1 from the lab instructions: (incoming light colour) * (brdf) * (cosine)
        frag_colour = light_colour[0]*3 * vec3(lambertian_brdf()) * max(0.2f, (dot(normalize(light_direction), normal)));
    //frag_colour = vec3(0.3, 0.3, 0.3);
    }
}
