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
    },
    {
      "MAX": [
        1,
        1
             ],
     "MIN": [
        0,
        0
     ],
     "NAME": "pos",
     "TYPE": "point2D"
    },
    {
      "MAX": [
        10
    ],
      "MIN": [
        0
    ],
 "NAME": "scale",
 "TYPE": "float"
 }
  ]
}
*/


//sunflower seed fibonacci spiral

#ifdef GL_ES
precision mediump float;
#endif


const float dotSize = 0.003;
const float blur = 0.003;
const float radius = .011;
const vec3 black = vec3(0.0, 0.0, 0.0);
const vec3 white = vec3(1.0, 1.0, 1.0);

float phiPi = 5.083203692+cos(TIME*0.1*length(mouse-0.5));

void main( void ) {

	vec2 p=(vv_FragNormCoord-pos)*scale;

	vec3 col = vec3(0.0);
	float r = length(p)/radius;
	r = floor(r*r);
	float best = 10000.0;
	const float range = 9.0;
	for(float i = -range;i<=range;++i)
	{
		float rt = r+i;
		float a = 2.*phiPi*rt;
		rt = radius * sqrt(rt);
		vec2 seedPos = rt*vec2(cos(a), sin(a));
		best=min(best, distance(p, seedPos));
	}
	col = mix(white, black, smoothstep(dotSize, dotSize + blur, best));;
	gl_FragColor = vec4(col,1.0);
}