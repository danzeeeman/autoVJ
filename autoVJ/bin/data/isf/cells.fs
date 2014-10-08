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


// Optimized version (logos7@o2.pl) - only one sqrt

vec2 hash2(vec2 uv) {
	return fract(sin(mat2(15.23, 35.48, 74.26, 159.37) * uv) * 47658.23);
}

void main( void ) {

	vec2 uv = ( gl_FragCoord.xy / RENDERSIZE.xy );
	uv.x *= RENDERSIZE.x / RENDERSIZE.y;
	uv *= 8.0;
	
	vec2 g = floor(uv);
	vec2 f = fract(uv);
	float d = 1.0;
	for(int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
			vec2 b = vec2(i, j);
			vec2 v = b + hash2(g + b) - f;
			d = min(d, dot(v,v)); // old: d = min(d, length(v));
		}
	}
	
	float c = sqrt(d); // old: float c = d;

	gl_FragColor = vec4( c, c, c, 1.0 );

}