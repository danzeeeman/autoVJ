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


float band(vec2 pos, float amplitude, float frequency) {
	float wave = scale * amplitude * sin(2.0 * PI * frequency * pos.x ) / 2.05;
	float light = clamp(amplitude * frequency * 0.002, 0.001 + 0.001 / scale, 5.0) * scale / abs(wave - pos.y);
	return light;
}

void main( void ) {
	vec2 vv_FragNormCoord = vec2(vv_FragNormCoord.x * 2.+TIME*3e-3, vv_FragNormCoord.y );
	vec3 color = vec3(0.3, 0.6, 0.9);
	color = color == vec3(0.0)? vec3(0.5, 0.5, 1.0) : color;
	vec2 pos = vv_FragNormCoord;//(gl_FragCoord.xy / RENDERSIZE.xy);
	//pos.y += - 0.5 - position;
	
	// +pk
	float spectrum = 0.0;
	const float lim = 128.;
	#define TIME TIME*0.00000 + pos.x*10.
	for(float i = 0.; i < lim; i++){
		spectrum += band(pos, 1.0*sin(TIME*0.1), 1.0*sin(TIME*i/lim))/pow(lim, 0.420 + 0.133*cos(TIME*3e3+TIME));
	}
	
	//spectrum += band(pos, 0.7, 2.5);
	//spectrum += band(pos, 0.4, 2.0);
	//spectrum += band(pos, 0.05, 4.5);
	//spectrum += band(pos, 0.1, 7.0);
	//spectrum += band(pos, 0.1, 1.0);
	
	
	
	gl_FragColor = vec4(color * spectrum, spectrum);
	
}