/*
{
  "CATEGORIES": [
    "Automatically Converted"
  ],
  "INPUTS": [
    {
			"NAME": "center",
			"TYPE": "point2D",
			"DEFAULT": [
				0.5,
				0.5
			],
			"MIN": [0.0, 0.0],
      "MAX": [1.0, 1.0]
		},
		 {
			"NAME": "rate",
			"TYPE": "point2D",
			"DEFAULT": [
				0.5,
				0.5
			],
			"MIN": [0.0, 0.0],
      "MAX": [0.5, 0.5]
		},
				 {
			"NAME": "cutoff",
			"TYPE": "point2D",
			"DEFAULT": [
				0.3,
				0.5
			],
			"MIN": [0.0, 0.0],
      "MAX": [1.0, 1.0]
		},
		{
			"NAME": "colorR",
			"TYPE": "float",
			"DEFAULT": 1.0,
			"MIN": 0.0,
      "MAX": 1.0
		},
		{
			"NAME": "colorG",
			"TYPE": "float",
			"DEFAULT": 0.5,
			"MIN": 0.0,
      "MAX": 1.0
		},
		{
			"NAME": "colorB",
			"TYPE": "float",
			"DEFAULT": 0.0,
			"MIN": 0.0,
      "MAX": 1.0
		},
			{
			"NAME": "pos",
			"TYPE": "float",
			"DEFAULT": 0.0,
			"MIN": -3.0,
      "MAX": 3.0
		}
  ]
}
*/


#ifdef GL_ES
precision mediump float;
#endif


void main(void) {
	float color = 0.0;
	
	float dist = length(vv_FragNormCoord.xy-center)*2.0;
	if(dist > cutoff.x && dist < cutoff.y) {
		float rot = pos * rate.x;
		vec2 position = (gl_FragCoord.xy/RENDERSIZE.xy) * mat2(cos(rot), sin(rot), -sin(rot), cos(rot));
		
		float angle = atan(position.x, position.y)*sin(pos+rate.x);
		color = (2.0 - abs(angle) * abs(sin(pos+rate.y)*4.0)) * (1.0 - abs(0.4 - dist) * 10.0);
	}

	gl_FragColor = vec4(vec3(color*vec3(colorR, colorG, colorB)), 1.0);
}