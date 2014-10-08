/*{
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
	if(dist > 0.3 && dist < 0.5) {
		float rot = TIME * 0.1;
			vec2 position = (gl_FragCoord.xy/RENDERSIZE.xy) * mat2(cos(rot), sin(rot), -sin(rot), cos(rot));
		
		float angle = atan(position.x, position.y)*sin(TIME*.0025);
		color = (2.0 - abs(angle) * abs(sin(TIME*0.05)*4.0)) * (1.0 - abs(0.4 - dist) * 10.0);
	}

		gl_FragColor = vec4(vec3(color*vec3(colorR, colorG, colorB)), 1.0);
}