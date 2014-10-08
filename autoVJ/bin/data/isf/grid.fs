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


vec3 color = vec3(0, 0, 0);

void main( void ) {

	vec2 position = ( gl_FragCoord.xy / RENDERSIZE.xy );
	
	color.xyz += 0.5;
	color.xyz /= (degrees(cos(position.x * 128.) + sin(TIME)) / 16.);
	color.xyz /= (degrees(sin(position.y * 128.) + sin(TIME)) / 16.);
	
	gl_FragColor = vec4(color.x, color.y, color.z, 1.0);

}