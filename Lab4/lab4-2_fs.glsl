#version 400
out vec4 o_fragment_color;

uniform sampler2D i_texture;
uniform bool i_display;
uniform uint i_frame_count;

uniform vec2 i_window_size;
uniform float i_global_time;
uniform vec4 i_mouse_state;

uniform vec3 i_position;
uniform vec3 i_up;
uniform vec3 i_right;
uniform vec3 i_dir;
uniform vec3 i_light_position;
uniform vec3 i_light_color;
uniform float i_focal_dist;

#define NUM_SPHERES 5
#define MAX_DEPTH 10
#define MAX_SAMPLES 1

struct Ray { vec3 origin, dir; float weight;};

//-------------------------------------------------------------------------//
// Keep a global stack of rays for recursion

Ray ray_stack[MAX_DEPTH];
int ray_stack_size = 0;

void push( Ray ray )
{
  if (ray_stack_size < MAX_DEPTH)
  {
    ray_stack[ray_stack_size] = ray;
    ray_stack_size++;
  }
  //else stack overflow -- silently ignore
}

Ray pop() // This is included for completeness, but actually isn't needed.
{
  if (ray_stack_size > 0)
  {
    ray_stack_size--;
    return ray_stack[ray_stack_size];
  }
  // else stack underflow -- silently ignore
}

//-------------------------------------------------------------------------//

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
	int hit;
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

void init( )
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
	scene.spheres[0].material.transmission = 0.3;
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
	scene.ground_plane[0].material.reflection = 0.3;
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
  vec3 light_color = clamp(i_light_color,0,1);

  float sun_spread = 2500.0;
  float haze_spread = 1.3;
  float elevation = acos(direction.y);
    
  float angle = abs(dot(direction, normalize(i_light_position)));
  float response_sun = pow(angle, sun_spread);
  float response_haze = pow(elevation, haze_spread);

  vec3 sun_component = mix(emission_sky*sky_color, emission_sun*light_color,response_sun);
  vec3 haze_component = mix(vec3(0),  emission_sky*haze_color,response_haze);

  return (sun_component+haze_component);
}


vec3 blinn_phong_brdf(vec3 in_direction, vec3 out_direction, Intersection I)
{
  // YOUR CODE GOES HERE
  // Implement a Blinn-phong BRDF
    //fr = kL (pL ...) + kg (pg ...)(n * h)

    float kL = 0.5f;
    float kg = 0.5f; 
    vec3 h = normalize(in_direction+out_direction);
    vec3 pL = I.material.color_diffuse;
    vec3 pg = I.material.color_glossy;
    float s = I.material.roughness;

    vec3 fr = kL*(pL) + kg*(pg*((8+s)/(8))*(pow(max(0.0,dot(I.normal, h)),s)));
    
    return fr;
}
// Ray-sphere intersection
float intersect(Ray ray, Sphere s) 
{
	//COPY YOUR CODE FROM 4.1 HERE
	// Return closest distance t for a ray/sphere intersection.
	vec3 v = ray.origin - s.center;
	vec3 omega = ray.dir;

	float a = dot(omega, omega);
	float b = dot(2 * omega, v);
	float c = dot(v,v) - pow(s.radius, 2);
	float t_plus = (-b + sqrt(pow(b,2) - 4*a*c)) / (2*a); 
	float t_minus = (-b - sqrt(pow(b,2) - 4*a*c)) / (2*a); 

	float whatevs = min(t_plus, t_minus);
	if (whatevs >= 0)
		return whatevs;
	else
		return 1e32;
}

// Ray-plane intersection
float intersect(Ray ray, Plane p) 
{
	//COPY YOUR CODE FROM 4.1 HERE
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

// Check for intersection of a ray and all objects in the scene
Intersection intersect( Ray ray)
{
  //COPY YOUR CODE FROM 4.1 HERE
  // Manage intersections
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
		I.hit = 1;

    }
  }

  //Check for intersection with planes
  {
    float d = intersect(ray,scene.ground_plane[0]);
    if (d>0 && d<=t) // if the plane is closer than `t`, update `t`
    {
		t=d;

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
		I.hit = 1;
      
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
	I.hit = 0;

  }
  return I;
}

vec3 raytrace() 
{
  vec3 color = vec3(0);

  {
    // Iterate over all rays on the stack
    for(int ray_stack_pos=0; ray_stack_pos < ray_stack_size; ++ray_stack_pos)
    {

      Ray ray = ray_stack[ray_stack_pos];
	  float contribution = ray.weight;
      Intersection isec = intersect(ray);

      vec3 nl = isec.normal * sign(-dot(isec.normal, ray.dir)); 
      vec3 light_direction = normalize(i_light_position - isec.point);

      vec3 this_color = vec3(0);
            
      float reflectivity = isec.material.reflection;
	  float transparency = isec.material.transmission;


      if (isec.material.transmission > 0)
      {
        // YOUR TASK: Create new ray, compute its position, its
        // direction (based on isec.material.ior) and its weight, and
        // call push(new_ray). In the next iteration of the for loop,
        // the new ray will be handled, so no recursive call to
        // raytrace() is required.

        // Optionally, compute what fraction should be reflected, and
        // send out a second ray in the reflection
        // direction. Otherwise, use the block below for specular
        // reflection.

		Ray ray2 = ray;
		float check_dot = dot(ray.dir, isec.normal);
		if( check_dot > 0 )
			ray2.dir = normalize(refract(ray.dir, -isec.normal, isec.material.ior/1));
		else
			ray2.dir = normalize(refract(ray.dir, isec.normal, 1/isec.material.ior));
		ray2.origin = isec.point + 0.001 * ray2.dir;
		ray2.weight = ray.weight * transparency;
		push( ray2 );
		contribution -= ray2.weight;

      }
      
      if (isec.material.reflection > 0)
      {
        // YOUR TASK: Create new ray, compute its position, direction,
        // and weight, and call push(ray).

		Ray ray2 = ray;
		ray2.dir = normalize(reflect(ray.dir, isec.normal));
		ray2.origin = isec.point + 0.001 * ray2.dir;
		ray2.weight = ray.weight * reflectivity;
		push( ray2 );
		contribution -= ray2.weight;

      }

      
      // Now handle non-specular scattering (i.e., the non-recursive case)
      {

        // YOUR TASK: Create a "shadow feeler" ray and check if this
        // point is visible from the (single) light source.
        // If it is in shadow, set a black (or dark "ambient") colour.

		Ray ray_shadow = ray;
		ray_shadow.dir = normalize(scene.sun_position - isec.point);
		ray_shadow.origin = isec.point + 0.001 * ray_shadow.dir;
		Intersection isec_shadow = intersect(ray_shadow);

		vec3 light_direction = normalize(scene.sun_position - isec.point);
		vec3 camera_direction = normalize(i_position - isec.point);

		if( isec_shadow.hit == 0 || isec.material.color_emission == vec3(1, 0, 0))
			this_color += (isec.material.color_emission + blinn_phong_brdf(light_direction, camera_direction, isec)) * max(0.2f, (dot(normalize(light_direction), normalize(isec.normal))));

		// YOUR TASK: If the point is not in shadow, compute the
        // colour here (using your BRDF, as before).
		this_color *= contribution;

		color += this_color;

      }


    }
  }

  return color;
}

void main() {

  vec2 tex_coords = gl_FragCoord.xy / i_window_size.xy;

  if(i_display)    
  {
    o_fragment_color = texture(i_texture,tex_coords);
  }    
  else
  {

    init();

			
	//crude zooming by pressing right mouse button
	float f_dist = i_focal_dist + i_focal_dist*i_mouse_state.w; 
	
	//basis for defining the image plane
	vec3 cx = i_right;
	vec3 cy = i_up;   
	vec3 cz = i_dir;  

    // Create a ray
    Ray ray;
	ray.origin = i_position;
	ray.dir = normalize(cz * i_focal_dist + cx * (gl_FragCoord.x - i_window_size.x/2) + cy * (gl_FragCoord.y - i_window_size.y/2));
    ray.weight = 1;
	// Push the ray noto the ray stack
    push( ray );
    
    vec3 color = raytrace(); 

    // gamma corrected output color, and blended over several frames (good for path tracer)
    o_fragment_color = (texture(i_texture,tex_coords)*i_frame_count + vec4( pow ( clamp(color.xyz/MAX_SAMPLES, 0., 1.), vec3(1./2.2)), 1.))/float(1+ i_frame_count); 

  }
}
