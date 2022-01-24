#version 400
out vec4 o_fragment_color; // The final colour

// User input:
uniform vec2 i_window_size;
uniform float i_global_time;
uniform vec4 i_mouse_state;

// Camera parameters:
uniform vec3 i_position;
uniform vec3 i_up;
uniform vec3 i_right;
uniform vec3 i_dir;
uniform float i_focal_dist;

#define NUM_SPHERES 5
#define MAX_SAMPLES 1

struct Ray {
	vec3 origin;
	vec3 dir;
};

struct Material{
  vec3 color_emission;
  vec3 color_diffuse;
  vec3 color_glossy;
  float roughness;
  float reflection;
  float transmission;
  float ior;  
};

struct Intersection
{
	vec3 point;
	vec3 normal;
	Material material;
};

struct Sphere {
  float radius;
  vec3 center;
  Material material;
};

struct Plane {
  float offset;
  vec3 normal;
  Material material;  
};

struct Scene {
  Sphere spheres[NUM_SPHERES];
  Plane ground_plane[1];
	vec3 sun_position;
  float sun_brightness;
};




Scene scene;


void init( float sun_bright )
{
	// Hard-coded single point light source
	scene.sun_brightness = 1;
	scene.sun_position = vec3(6e3,  1e4, 1e4);
	
	// Initialise 5 spheres and a ground plane

	scene.spheres[0].center = vec3(0, 0.3, 0.5) ; 
	scene.spheres[0].radius = 0.3;
	scene.spheres[0].material.color_diffuse = vec3( 0.3, 0.1, 0.1 );
	scene.spheres[0].material.color_glossy = vec3( 1 );
	scene.spheres[0].material.color_emission = vec3( 0 );
	scene.spheres[0].material.roughness = 100;
	scene.spheres[0].material.reflection = 0.5; //reflective red ball
	scene.spheres[0].material.transmission = 0;
	scene.spheres[0].material.ior = 1;
  
	scene.spheres[1].center = vec3(0.8, 0.3, 0.8);
	scene.spheres[1].radius = 0.3;
	scene.spheres[1].material.color_diffuse = 0.5 * vec3( 0.0, 1.0, 0.0 );
	scene.spheres[1].material.color_glossy = vec3( 1 );
	scene.spheres[1].material.roughness = 10000;
	scene.spheres[1].material.color_emission = vec3( 0 );
	scene.spheres[1].material.reflection = 0.1;
	scene.spheres[1].material.transmission = 0.8; // glass green ball
	scene.spheres[1].material.ior = 1.4;

	scene.spheres[2].center = vec3(0.55, 0.1, 0.2) ;
	scene.spheres[2].radius = 0.1;
	scene.spheres[2].material.color_diffuse = 0.8 * vec3( 1.0, 0.0, 0.0 );
	scene.spheres[2].material.color_glossy = vec3( 1 );
	scene.spheres[2].material.roughness = 1000;
	scene.spheres[2].material.color_emission = vec3( 1, 0, 0 ); // glowing red ball
	scene.spheres[2].material.reflection = 0.0;
	scene.spheres[2].material.transmission = 0;
	scene.spheres[2].material.ior = 1;

	scene.spheres[3].center = vec3(0.7, 0.8, -0.5) ;
	scene.spheres[3].radius = 0.8;
	scene.spheres[3].material.color_diffuse = 0.5 * vec3( 0.2, 0.2, 0.15 );
	scene.spheres[3].material.color_glossy = vec3( 1 );
	scene.spheres[3].material.roughness = 10;
	scene.spheres[3].material.color_emission = vec3( 0 );
	scene.spheres[3].material.reflection = 0.0;
	scene.spheres[3].material.transmission = 0;
	scene.spheres[3].material.ior = 1;

	scene.spheres[4].center = vec3(-0.65, 0.6, -1) ;
	scene.spheres[4].radius = 0.6;
	scene.spheres[4].material.color_diffuse = 0.5 * vec3( 0.5, 0.4, 0.25 );
	scene.spheres[4].material.color_glossy = vec3( 1 );
	scene.spheres[4].material.roughness = 5000;
	scene.spheres[4].material.color_emission = vec3( 0 );
	scene.spheres[4].material.reflection = 0.0;
	scene.spheres[4].material.transmission = 0;
	scene.spheres[4].material.ior = 1;

	scene.ground_plane[0].normal = vec3(0,1,0);
	scene.ground_plane[0].offset = 0;
	scene.ground_plane[0].material.color_diffuse = 1.0 * vec3( 0.6 );
	scene.ground_plane[0].material.color_glossy = vec3( 0 );
	scene.ground_plane[0].material.roughness = 0;
	scene.ground_plane[0].material.color_emission = vec3( 0 );
	scene.ground_plane[0].material.reflection = 0.0;
	scene.ground_plane[0].material.transmission = 0;
	scene.ground_plane[0].material.ior = 1;  
}


// This function computes a nice-looking sky sphere, with a sun.
vec3 simple_sky(vec3 direction)
{
	float emission_sky = 1*scene.sun_brightness;
	float emission_sun = 10*scene.sun_brightness*scene.sun_brightness;
  vec3 sky_color = vec3(0.35, 0.65, 0.85);
  vec3 haze_color = vec3(0.8, 0.85, 0.9);
  vec3 light_color = vec3(1);

  float sun_spread = 2500.0;
  float haze_spread = 1.3;
  float elevation = acos(direction.y);
    
  float angle = abs(dot(direction, normalize(scene.sun_position)));
  float response_sun = pow(angle, sun_spread);
  float response_haze = pow(elevation, haze_spread);

  vec3 sun_component = mix(emission_sky*sky_color, emission_sun*light_color,response_sun);
  vec3 haze_component = mix(vec3(0),  emission_sky*haze_color,response_haze);

  return (sun_component+haze_component);
}


float intersect(Ray ray, Sphere s) 
{
	///\todo YOUR CODE GOES HERE.
	// Return closest distance t for a ray/sphere intersection.
	vec3 v = ray.origin - s.center;
	vec3 omega = ray.dir;

	float a = dot(omega, omega);
	float b = dot(2 * omega, v);
	float c = dot(v,v) - pow(s.radius, 2);
	float t_plus = (-b + sqrt(pow(b,2) - 4*a*c)) / (2*a); 
	float t_minus = (-b - sqrt(pow(b,2) - 4*a*c)) / (2*a); 

	if( t_plus >= 0 && t_minus >= 0)
		return min(t_plus, t_minus);
	else if(  t_plus >= 0 )
		return t_plus;
	else if(  t_minus >= 0 )
		return t_minus;
	else
		return 1e32; //"Infinitely far away"
}

float intersect(Ray ray, Plane p) 
{
  ///\todo YOUR CODE GOES HERE.
  // Return closest distance t for a ray/plane intersection.
	float d = p.offset;
	vec3 p0 = ray.origin;
	vec3 omega = ray.dir;
	float t = (d - dot(p.normal, p0)) / dot(p.normal, omega);
	if( t >= 0 )
		return t;
	else
		return 1e32; //"Infinitely far away"
}


Intersection intersect( Ray ray)
{
  Intersection I;
  float t = 1e32; // closest hit so far along this ray
  int id = -1;
    
  //Check for intersection with spheres
  for (int i = 0; i < NUM_SPHERES; ++i) {
    float d = intersect(ray, scene.spheres[i]);
    if (d>0 && d<=t) // if sphere `i` is closer than `t`, update `t` and `id`
    {
		t = d; 
		id = i;

		///\todo YOUR CODE GOES HERE
		// Populate I with all the relevant data.  `id` is the closest
		// sphere that was hit, and `t` is the distance to it.
		I.point = ray.origin + ray.dir*t;
		I.normal = normalize(I.point - scene.spheres[i].center);

		I.material.color_diffuse = scene.spheres[i].material.color_diffuse;
		I.material.color_glossy = scene.spheres[i].material.color_glossy;
		I.material.roughness = scene.spheres[i].material.roughness;
		I.material.color_emission = scene.spheres[i].material.color_emission;
		I.material.reflection = scene.spheres[i].material.reflection;
		I.material.transmission = scene.spheres[i].material.transmission;
		I.material.ior = scene.spheres[i].material.ior;

    }
  }

  //Check for intersection with planes
  {
    float d = intersect(ray,scene.ground_plane[0]);
    if (d>0 && d<=t) // if the plane is closer than `t`, update `t`
    {
		t=d;

		///\todo YOUR CODE GOES HERE
		// Populate I with all the relevant data.
		I.point = ray.origin + ray.dir*t;
		I.normal = normalize(scene.ground_plane[0].normal);

		I.material.color_diffuse = scene.ground_plane[0].material.color_diffuse;
		I.material.color_glossy = scene.ground_plane[0].material.color_glossy;
		I.material.roughness = scene.ground_plane[0].material.roughness;
		I.material.color_emission = scene.ground_plane[0].material.color_emission;
		I.material.reflection = scene.ground_plane[0].material.reflection;
		I.material.transmission = scene.ground_plane[0].material.transmission;
		I.material.ior = scene.ground_plane[0].material.ior;
      
      // Adding a procedural checkerboard texture:
      I.material.color_diffuse = (mod(floor(I.point.x) + floor(I.point.z),2.0) == 0.0) ?
        scene.ground_plane[0].material.color_diffuse :
        vec3(1.0) - scene.ground_plane[0].material.color_diffuse;
    }
  }

  //If no sphere or plane hit, we hit the sky instead
  if (t>1e20){
    I.point = ray.dir*t;
    I.normal = -ray.dir;
    vec3 sky = simple_sky(ray.dir); // pick color from sky function

    // Sky is all emission, no diffuse or glossy shading:
    I.material.color_diffuse = 0 * sky; 
    I.material.color_glossy = 0.0 * vec3( 1 );
    I.material.roughness = 1;
    I.material.color_emission = 0.3 * sky;
    I.material.reflection = 0.0;
    I.material.transmission = 0;
    I.material.ior = 1;

  }
  return I;
}



vec3 F(vec3 n, vec3 l, Intersection I){
	
	vec3 F0 = I.material.color_diffuse;

	vec3 fresnel = F0 + (1-F0) * pow((1-max(0.0f, (dot(normalize(n), normalize(l))))), 5);

	return fresnel;
}

float D(vec3 m, vec3 normal){

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

vec3 cook_torrance(vec3 light_dir, vec3 camera_dir, Intersection I){

	vec3 h = normalize(light_dir+camera_dir);

	
    float kL = 0.5f;
    float kg = 0.5f; 
    vec3 pL = I.material.color_diffuse;
	
	vec3 norm = normalize(I.normal); 

    vec3 cook_torr = kL*(pL) + kg*((F(h, light_dir, I)*G(light_dir, camera_dir, norm)*D(h, norm))/ (4*(abs(dot(norm, light_dir))*(abs(dot(norm, camera_dir))))));

	return cook_torr;
}


vec3 raycast(Ray ray) 
{
  vec3 color = vec3(0);

  Intersection isec = intersect(ray);

  ///\todo YOUR CODE GOES HERE: Compute a more interesting colour instead of
  // the one below!  Compute the shading, using the
  // position/normal/material data stored in isec; and
  // i_light_position, which is a point light source at the sun.
  vec3 light_direction = normalize(scene.sun_position - isec.point);
  vec3 camera_direction = normalize(i_position - isec.point);

  //check for sky, good ? 
	if(isec.material.color_diffuse == vec3(0, 0, 0))
		color = isec.material.color_diffuse + isec.material.color_emission;

	else
		color = isec.material.color_emission + vec3(1, 1, 1)*2 * (cook_torrance(light_direction, camera_direction, isec) * max(0.2f, (dot(normalize(light_direction), normalize(isec.normal)))));

	return color;
}

void main() {
	vec2 uv =  gl_FragCoord.xy - 0.5*i_window_size.xy;
	
	//init(i_light_position, 10.0 ); //i_mouse_state.y); 
	init( 10.0 );
	
	int num_samples=1;

		
	//crude zooming by pressing right mouse button
	float f_dist = i_focal_dist + i_focal_dist*i_mouse_state.w; 
	
	//basis for defining the image plane
	vec3 cx = i_right;
	vec3 cy = i_up;   
	vec3 cz = i_dir;  

	Ray ray;
	ray.origin = i_position;
	///\todo YOUR CODE GOES HERE: Compute the correct ray direction using gl_fragCoord and the camera vectors above.
	//ray.dir = vec3(0,-1,0);
	ray.dir = normalize(cz * i_focal_dist + cx * (gl_FragCoord.x - i_window_size.x/2) + cy * (gl_FragCoord.y - i_window_size.y/2));
	vec3 color = raycast(ray);
	
	//linear blend
	//o_fragment_color =  vec4((color),1);

	//gamma correction
	o_fragment_color = vec4( pow ( clamp(color.xyz/num_samples, 0., 1.), vec3(1./2.2)), 1.); 

}


