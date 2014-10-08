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


void main( void ) {

	vec2 position = ( gl_FragCoord.xy / RENDERSIZE.x );

	float color = pow(length(position-vec2(0.5,0.25)),cos(TIME)+0.5*atan(TIME));
	gl_FragColor = vec4( vec3( color,cos(color)*0.5/0.5,(atan(color*100.0))), 1.0 );

}