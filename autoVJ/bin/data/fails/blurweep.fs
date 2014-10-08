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


float PI = atan(1.0)*4.0;

vec3 color(float d) 
{
	return d * vec3(2.0, 2.0, 3.0);	
}

void main(void)
{
	float sineCheck = atan(TIME);
	vec2 p = (-1.0 + 2.0 * ((gl_FragCoord.xy) / RENDERSIZE.xy));
	p.x *= (RENDERSIZE.x / RENDERSIZE.y);

	float a = (atan(p.y,sineCheck*p.x) + TIME);
	float r = dot(p,p);

	vec3 col = color(pow(fract(a/PI*1.0 / -2.0), 1.0)) * (1.0 - smoothstep(0.0 + (cos(TIME)*0.33), 1.0, r));

	gl_FragColor = vec4(col, 1.0);
}