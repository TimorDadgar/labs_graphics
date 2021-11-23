#version 440

out vec3 frag_colour;
in vec3 position;
in vec3 normal;

uniform int light_count;
uniform vec3 light_position[4]; //add [4] // can have up to 4 light sources
uniform vec3 light_colour[4]; //add [4]


vec3 F(vec3 n, vec3 l){
	
	vec3 F0 = vec3(1, 0.78, 0.34);

	vec3 fresnel = F0 + (1-F0) * pow((1-max(0.0f, (dot(normalize(n), normalize(l))))), 5);

	return fresnel;
}

float D(vec3 m){

	int x_scalar = 1;
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

	
    float kL = 0.2f;
    float kg = 0.8f; 
    vec3 pL = vec3(1, 0.78, 0.34);
	vec3 norm = normalize(normal); 

    vec3 cook_torr = kL*(pL) + kg*((F(h, light_dir)*G(light_dir, camera_dir, norm)*D(h))/ (4*(abs(dot(norm, light_dir))*(abs(dot(norm, camera_dir))))));

	return cook_torr;
}

void main () {

	vec3 light_direction = light_position[0] - position;
	vec3 camera_direction = vec3(0.0f, 0.0f, 0.0f) - position;
 
	for ( int l = 0; l < light_count; ++l )
	{
		frag_colour = light_colour[0]*2 * cook_torrance(light_direction, camera_direction) * max(0.2f, (dot(normalize(light_direction), normalize(normal))));
	}
}
