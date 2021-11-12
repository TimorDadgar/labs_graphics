#version 440

out vec3 frag_colour;
in vec3 position;
in vec3 normal;

uniform int light_count;
uniform vec3 light_position[4]; //add [4] // can have up to 4 light sources
uniform vec3 light_colour[4]; //add [4]
uniform vec3 view_vector[4];


vec3 blinn_phong_brdf(vec3 in_direction, vec3 out_direction, vec3 normal)
{
  // YOUR CODE GOES HERE
  // Implement a Blinn-phong BRDF
    //fr = kL (pL ...) + kg (pg ...)(n * h)

    float kL = 0.9f;
    float kg = 0.1f; 
    vec3 h = normalize((in_direction+out_direction)/abs(in_direction+out_direction));
    vec3 pL = vec3(0.8, 0.5, 0.3);
    vec3 pg = pL;
    int s = 100;

    vec3 fr = kL*(pL) + kg*(pg*((8+s)/(8*3.1412))*(pow(dot(normal, h),s)));
    
    return fr;
}

void main () {

    vec3 light_direction = light_position[0] - position;
    vec3 camera_direction = view_vector[0] - position;
 
    for ( int l = 0; l < light_count; ++l )
    {
    // YOUR CODE GOES HERE
    // Implement Equation 1 from the lab instructions: (incoming light colour) * (brdf) * (cosine)
        frag_colour = light_colour[0] * blinn_phong_brdf(light_direction, camera_direction, normalize(normal)) * max(0.2f, (dot(normalize(light_direction), normalize(normal))));
        //frag_colour = blinn_phong_brdf(light_direction, camera_direction, normal);
    }
}
