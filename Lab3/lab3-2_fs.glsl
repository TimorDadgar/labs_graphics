// Build on your code from Lab 1 here
#version 400
in vec3 position;
in vec3 normal;
in vec2 uv_coords;
out vec3 frag_colour;
uniform sampler2D tex_sampler;
uniform samplerCube env_sampler;

uniform int light_count;
uniform vec3 light_position[4]; //add [4] // can have up to 4 light sources
uniform vec3 light_colour[4]; //add [4]

vec3 normal_n = normalize(normal);

vec3 F(vec3 n, vec3 l){
	
	vec3 F0 = vec3(1, 0.78, 0.34);

	vec3 fresnel = F0 + (1-F0) * pow((1-max(0.0f, (dot(normalize(n), normalize(l))))), 5);

	return fresnel;
}

float D(vec3 m){

	int x_scalar = 0;
	float alpha = 0.3;

	float n_scalar_m = dot(normalize(normal), normalize(m));

	if(n_scalar_m >= 0)
		x_scalar = 1;
	else
		x_scalar = 0;

	float alpha_temp = x_scalar / (3.1415*pow(alpha,2)*pow(n_scalar_m,4));
	float exp_temp = exp((pow(n_scalar_m, 2)-1) / (pow(alpha, 2) * pow(n_scalar_m,2)));

	float NDF = alpha_temp * exp_temp;

	return NDF;
}

float G(vec3 light_dir, vec3 camera_dir, vec3 norm){

	vec3 h = normalize(light_dir+camera_dir);

	float g_temp = min(1, 2*(dot(norm, h)*(dot(norm, camera_dir)) / (dot(camera_dir, h))));
	float smith_G = min(g_temp , 2*(dot(norm, h)*(dot(norm, light_dir)) / (dot(camera_dir, h))));

	return smith_G;
}

vec3 cook_torrance(vec3 light_dir, vec3 camera_dir){

	vec3 h = normalize(light_dir+camera_dir);
	//Number texture
	vec4 tex = texture(tex_sampler, uv_coords);

    float kL = 0.2f;
    float kg = 0.8f; 
    vec3 pL = vec3(0.3, 0.3, 1);
	vec3 norm = normalize(normal); 

    vec3 cook_torr = tex.rgb *  kL*(pL) + kg*((F(h, light_dir)*G(light_dir, camera_dir, norm)*D(h))/ (4*(abs(dot(norm, light_dir))*(abs(dot(norm, camera_dir))))));

	return cook_torr;
}


void main () {
	
	vec3 light_direction = normalize(light_position[0] - position);
	vec3 camera_direction = normalize(vec3(0.0f, 0.0f, 0.0f) - position);

	//Reflection texture
	vec3 reflection_vector = reflect(camera_direction, normal_n);
	vec4 tex2 = texture(env_sampler, reflection_vector);

	float r = 0.0;

	for ( int l = 0; l < light_count; ++l )
	{
		frag_colour = light_colour[0]*3 * cook_torrance(light_direction, camera_direction) * max(0.2f, (dot(normalize(light_direction), normalize(normal))));
	}

	frag_colour = r*vec3(tex2)+ (1-r)*frag_colour;
}

