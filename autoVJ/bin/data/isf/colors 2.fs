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

#define PI 3.1415926535897932384626433832795


vec2 pos;

float plasma1() {
	return sin((10.0*pos.x) + TIME);
}

float plasma2() {
	return sin(10.0*(pos.x*sin(TIME/2.0) + pos.y*cos(TIME/3.0)) + TIME);
}

float plasma3() {
	float centerX = pos.x + 0.5*sin(TIME/5.0);//kjkjk
	float centerY = pos.y + 0.5*cos(TIME/3.0);
	return sin(sqrt(100.0*(centerX*centerX + centerY*centerY) + 1.0) + TIME);
}

void main( void ) {
	pos = gl_FragCoord.xy / RENDERSIZE;	
	float plasma = plasma1() + plasma2() + plasma3();
	gl_FragColor = vec4(sin(plasma*PI), cos(plasma*PI), 0, 1.0);
}