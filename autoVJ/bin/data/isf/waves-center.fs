/*
{
  "CATEGORIES": [
    "Automatically Converted"
  ],
  "INPUTS": [
    
  ]
}
*/


// Tenjix

#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.1415926535897932384626433832795


const float position = 0.0;
const float scale = 1.0;
const float intensity = 1.0;

#define TIME time*0.01 + length(vv_FragNormCoord)*2.

float band(vec2 pos, float amplitude, float frequency) {
	float wave = scale * amplitude * sin(TIME+2.0 * PI * frequency * pos.x) / 2.05;
	float light = clamp(amplitude * frequency * 0.002, 0.002 + 0.001 / scale, 5.0) * scale / abs(wave - pos.y);
	return light;
}

void main( void ) {

	vec3 color = vec3(0.5, 0.5, 1.0);
	color = color == vec3(0.0)? vec3(0.5, 0.5, 1.0) : color;
	vec2 pos = vv_FragNormCoord;//(gl_FragCoord.xy / RENDERSIZE.xy);pos.y += - 0.5 - position;
	
	float spectrum = 0.0;
	
	//+pk
	const float lim = 100.;
	for(float i = 0.; i < lim; i++){
		float r = i/lim;
		float w = 12./lim;
		spectrum += band(pos, r, r)*w;
		spectrum += band(pos, 1./r, r)*w;
		spectrum += band(pos, r, -r)*w;
		spectrum += band(pos, 1./r, -r)*w;
	}
	
	gl_FragColor = vec4(color * spectrum, spectrum);
	
}