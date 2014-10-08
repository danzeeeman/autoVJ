/*{
  "CREDIT": "by thedantheman",
  "DESCRIPTION": "",
  "CATEGORIES": [
  ],
  "INPUTS": [
{
			"NAME": "offsetX",
			"TYPE": "float",
			"MIN": -2.0,
			"MAX": 2.0,
			"DEFAULT": 0.5
		},
		{
			"NAME": "offsetY",
			"TYPE": "float",
			"MIN": -2.0,
			"MAX": 2.0,
			"DEFAULT": 0.5
		},
			{
			"NAME": "master_scale",
			"TYPE": "float",
			"MIN": 0.1,
			"MAX": 1.0,
			"DEFAULT": 0.5
		},
		{
			"NAME": "colorR",
			"TYPE": "float",
			"MIN": 0.0,
			"MAX": 1.0,
			"DEFAULT": 0.9
		},
				{
			"NAME": "colorG",
			"TYPE": "float",
			"MIN": 0.0,
			"MAX": 1.0,
			"DEFAULT": 0.5
		},
				{
			"NAME": "colorB",
			"TYPE": "float",
			"MIN": 0.0,
			"MAX": 1.0,
			"DEFAULT": 0.1
		},
				{
			"NAME": "intensity",
			"TYPE": "float",
			"MIN": 0.0,
			"MAX": 1.0,
			"DEFAULT": 0.1
		}
		,
				{
			"NAME": "MIN",
			"TYPE": "float",
			"MIN": 0.0,
			"MAX": 10.0,
			"DEFAULT": 0.1
		}
		,
				{
			"NAME": "MAX",
			"TYPE": "float",
			"MIN": 1.0,
			"MAX": 10.0,
			"DEFAULT": 0.1
		},
		{
			"NAME": "radius",
			"TYPE": "float",
			"MIN": 0.1,
			"MAX": 10.0,
			"DEFAULT": 1.0
		},
				{
			"NAME": "alphaScale",
			"TYPE": "float",
			"MIN": 0.1,
			"MAX": 2.0,
			"DEFAULT": 1.0
		}


  ]
}*/

#ifdef GL_ES
precision mediump float;
#endif

void main(void) {
	vec2 offset = vec2(-offsetX , -offsetY); // x / y offset
	vec3 light_color = vec3(colorR, colorG, colorB);
	float c = master_scale/(length(gl_FragCoord.xy/RENDERSIZE.x/radius+offset/radius));
	gl_FragColor = smoothstep(MIN,MAX,c) * vec4(intensity) + vec4(vec3(c) * light_color, smoothstep(MIN,MAX,c)/alphaScale);
}