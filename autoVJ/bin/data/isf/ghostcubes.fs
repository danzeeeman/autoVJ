/*
{
  "CATEGORIES": [
    "Automatically Converted"
  ],
  "INPUTS": [
    
  ]
}
*/


#ifdef GL_ES
precision mediump float;
#endif

// xray by Jaksa


float sphere(vec3 p, vec3 o, float r) {
	return distance(p,o) - r;
}

float cube(vec3 p, vec3 o, float side) {
	vec3 d = abs(p - o);
	return max(d.x, max(d.y, d.z)) - side;
}

float map(vec3 p) {
	p.z += TIME*20.0;
	p.x += -cos(TIME/4.)*10.;
	p.y += 30.*sin(TIME/10.)+10.;
	p.xz = mod(p.xz+10., 20.0)-10.;
	float d = p.y + 30.;
//	d = min(d, sphere(p, vec3(0,0,6), 3.0));
//	d = min(d, sphere(p, vec3(1,5,10), 2.0));
	d = min(d, cube(p, vec3(2,40,7), 4.0));
	d = min(d, cube(p, vec3(2,10,7), 2.0));
	d = min(d, cube(p, vec3(2,30,7), 1.0));
	return d;
}

vec3 grad(vec3 p) {
	vec2 d = vec2(0.01,0.0);
	
	return (map(p) - vec3(map(p + d.xyy), map(p + d.yxy), map(p+d.yyx)))/d.x;
}

void main( void ) {
	vec2 pix = (2.*gl_FragCoord.xy/RENDERSIZE.y-1.);
	vec3 cam = vec3(0,10.0,0);
	vec3 camDir = normalize(vec3(sin(TIME/4.)*.2,0,1));
	vec3 screenCenter = cam + camDir;
	vec3 up = vec3(0,1,0);
	vec3 u = -normalize(cross(up, camDir));
	vec3 v = normalize(cross(u, camDir));
	vec3 uv = u * pix.x + v * pix.y;
	vec3 p = vec3(uv+screenCenter);	
	vec3 dir = normalize(p-cam);

	float delta = 0.001;
	float d = 0.0;
	float dist = 0.0;
	
	float volume = 0.0;
	float angle = 0.0;
	
	for (int i = 0; i < 128; i++) {
		d = map(p);
		volume += max(-clamp(d, -.01, .01), 0.0);
		dist += max(max(0.0, d), .2);
		if (dist > 500.) break;
		p = dir*dist;
	}
	
	angle = dot(dir, grad(p));
	float c = log(1.0 + volume)/(1.0 + log(1.0 + volume));
	c *= 2.6;
	gl_FragColor = vec4(c * vec3(.6*c,.6*c,1.0), 1.0 );
}