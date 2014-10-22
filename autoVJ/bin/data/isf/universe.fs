
/*
 {
    "CATEGORIES": [
                   "Automatically Converted"
                   ],
    "INPUTS": [
               {
               "NAME": "scale",
               "TYPE": "float",
               "MIN": 0.0000001,
               "MAX": 100.0,
               "DEFAULT":0.5
               },
               {
               "NAME": "speed",
               "TYPE": "float",
               "MIN": 0.1,
               "MAX": 10.0,
               "DEFAULT": 0.15
               },
               {
               "NAME": "red",
               "TYPE": "float",
               "MIN": 0.0,
               "MAX": 1.0,
               "DEFAULT": 0.15
               },
               {
               "NAME": "blue",
               "TYPE": "float",
               "MIN": 0.0,
               "MAX": 1.0,
               "DEFAULT": 0.15
               },
               {
               "NAME": "green",
               "TYPE": "float",
               "MIN": 0.0,
               "MAX": 1.0,
               "DEFAULT": 0.15
               },
               {
               "NAME": "ZOOM",
               "TYPE": "float",
               "MIN": 0.0001,
               "MAX": 100.0,
               "DEFAULT": 0.0015
               },
               {
               "NAME": "offsetX",
               "TYPE": "float",
               "MIN": 0.0,
               "MAX": 1.0,
               "DEFAULT": 0.5
               },
               {
               "NAME": "offsetY",
               "TYPE": "float",
               "MIN": 0.0,
               "MAX": 1.0,
               "DEFAULT": 0.5
               }
               ,
               {
               "NAME": "glowA",
               "TYPE": "float",
               "MIN": 0.0,
               "MAX": 1.0,
               "DEFAULT": 0.5
               }
               ,
               {
               "NAME": "glowB",
               "TYPE": "float",
               "MIN": 0.0,
               "MAX": 1.0,
               "DEFAULT": 0.5
               }
               ]
    
}
 */



#ifdef GL_ES
precision mediump float;
#endif


// https://www.shadertoy.com/view/MdXSzS

void main()
{
	
	vec2 uv = (gl_FragCoord.xy/RENDERSIZE.xy)-vec2(offsetX,offsetY);
    
	float TIME = TIME * .01 + ((.25+sin(TIME*.1))/(length(uv.xy)))* speed;
	float si = sin(TIME*(1.0/scale));
	float co = cos(TIME*(1.0/scale));
	mat2 ma = mat2(co, si, -si, co);
    
	float c = 0.0;
	float v1 = 0.0;
	float v2 = 0.0;
	
	for (int i = 0; i < 100; i++)
	{
		float s = float(i) * (1.0/ZOOM);
		vec3 p = s * vec3(uv, 0.0);
		p.xy *= ma;
		p += vec3(.22,.3, s-1.5-sin(TIME*.13)*.1);
		for (int i = 0; i < 8; i++)
		{
			p = abs(p) / dot(p,p) - 0.659;
		}
		v1 += dot(p,p)*.0015 * (1.8+sin(length(uv.xy*13.0)+.5-TIME*.2));
		v2 += dot(p,p)*.0015 * (1.5+sin(length(uv.xy*13.5)+2.2-TIME*.3));
		c = length(p.xy*.5) * .35;
	}
	
	float len = length(uv);
	v1 *= smoothstep(glowA, .0, len);
	v2 *= smoothstep(glowB, .0, len);
	
	float re = clamp(c*red, 0.0, 1.0);
	float gr = clamp((v1+c)*green, 0.0, 1.0);
	float bl = clamp(v2*blue, 0.0, 1.0);
	vec3 col = vec3(re, gr, bl) + smoothstep(0.15, .0, len) * .9;
    
	gl_FragColor=vec4(col, 1.0);
}