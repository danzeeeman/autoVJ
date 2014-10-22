/*
{
  "CATEGORIES": [
    "Automatically Converted"
  ],
  "INPUTS": [
    {
      "MAX": [
        1,
        1
      ],
      "MIN": [
        0,
        0
      ],
      "NAME": "mouse",
      "TYPE": "point2D"
    }
  ]
}
*/


#ifdef GL_ES
precision highp float;
#endif


vec4 pal(float t) {
	return vec4(
		sin(t/2.0)+cos(t/5.76+14.5)*0.5+0.5,
		sin(t/2.0)+cos(t/4.76+14.5)*0.5+0.4,
		sin(t/2.0)+cos(t/3.76+14.5)*0.5+0.3,
		1.0);
}

void main( void ) {
	vec2 pos = gl_FragCoord.xy / RENDERSIZE;
	float aspect = RENDERSIZE.x / RENDERSIZE.y;

	float rand = mod(fract(sin(dot(pos + TIME, vec2(12.9898,100.233))) * 43758.5453), 1.0) * 0.1;
	rand += .8 * (1. - (length((pos - (1.0 -mouse)) * vec2(aspect, 1.)) * 8.));
        rand *= 1.8 * (1. - (length((pos - mouse) * vec2(aspect, 1.)) * (2.0+sin(TIME*1.0)*2.0)));

	//gl_FragColor = vec4( sin(rand*4.0), cos(rand*0.3), sin(10.0+rand*10.0), 1.0);
	gl_FragColor=pal(rand*4.0);
}