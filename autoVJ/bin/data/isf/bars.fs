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

float rand( float n)
{
	return fract(sin(n*323664.23356)*34446.536);
}
///Why not?
void main( void ) {

	// This is sexy. :]
	vec2 position = ( gl_FragCoord.xy / RENDERSIZE.x );
	float scale = 50.0;
	float height = rand(fract(floor(TIME*5.0)/100.0)+floor((position.x)*20.0));
	float color = 1.0-clamp(sign(position.y-height),0.0,1.0);
	gl_FragColor = vec4( vec3( color*floor((position.x*color)*20.0)/10.0,color*floor((position.x*color)*20.0)/20.0 ,color*0.25), 1.0 );
}