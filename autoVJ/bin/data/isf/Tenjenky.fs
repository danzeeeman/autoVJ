/*
{
  "CATEGORIES": [
    "Automatically Converted"
  ],
  "INPUTS": [
    
  ]
}
*/


// Tenjenky

#ifdef GL_ES
precision lowp float;
#endif

#define PI 3.1415926535897932384626433832795


const float position = 1.0;
const float scale = 1.0;
const float intensity = 1.0;


float band(vec2 pos, float amplitude, float frequency) {
	float wave = scale * amplitude * sin(2.0 * PI * frequency * pos.x ) / 2.05;
	float light = clamp(amplitude * frequency * 0.002, 0.001 + 0.001 / scale, 5.0) * scale / abs(wave - pos.y);
	return light;
}

void main( void ) {
	//vec2 vv_FragNormCoord = vec2(vv_FragNormCoord.x * 2.+TIME*3e-3, vv_FragNormCoord.y );
	vec3 color = vec3(0.3, 0.6, 0.9);
	color = color == vec3(0.0)? vec3(0.5, 0.5, 1.0) : color;
	float TIME = TIME + vv_FragNormCoord.x;
	vec2 pos = vv_FragNormCoord;//*vec2(0.2+0.001*TIME,sin(TIME*2.)*sin(TIME*2.)*pow(1e1,0.-cos(TIME)*cos(TIME)))+vec2(0.+TIME*.02,0.);//(gl_FragCoord.xy / RENDERSIZE.xy);
	//pos.y += - 0.5 - position;
	
	// +pk
	float spectrum = 0.0;
	const float lim = 256.;
	#define TIME TIME*0.1 + pos.x/4.0 + pos.y/(cos(TIME*0.05-pos.x*3.)*256.)
	float limAtten = pow(lim, 0.3);
	for(float i = 0.; i < lim; i++){
		spectrum += band(pos,
		  sin(TIME+i),
		  sin(TIME*(1.-i/lim))
		)/limAtten;
	}
	
	//spectrum += band(pos, 0.7, 2.5);
	//spectrum += band(pos, 0.4, 2.0);
	//spectrum += band(pos, 0.05, 4.5);
	//spectrum += band(pos, 0.1, 7.0);
	//spectrum += band(pos, 0.1, 1.0);
	
	
	
	gl_FragColor = vec4(color * spectrum, 1.0);
	
}