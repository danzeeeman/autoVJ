/*
{
  "CATEGORIES": [
    "Source",
    "dantheman hacks via https://glsl.heroku.com/e#19580.1."
  ],
  "INPUTS": [
    {
			"NAME": "mpos",
			"TYPE": "float",
			"MIN": 0,
			"MAX": 100.0,
			"DEFAULT": 0.0
		},
		{
			"NAME": "zoom",
			"TYPE": "float",
			"MIN": 0.1,
			"MAX": 1.0,
			"DEFAULT": 1.0
		},
		{
			"NAME": "center",
			"TYPE": "point2D",
			"MIN": [-1.0, -1.0],
			"MAX": [1.0, 1.0],
			"DEFAULT": [0.0, 0.0]
		},
		{
			"NAME": "warp",
			"TYPE": "float",
			"MIN": 0.0,
			"MAX": 10.0,
			"DEFAULT": 0.5
		},
			{
			"NAME": "phase",
			"TYPE": "float",
			"MIN": -1.0,
			"MAX": 1.0,
			"DEFAULT": 0.5
		}
  ]
}
*/


#ifdef GL_ES
precision mediump float;
#endif


#define PI 3.1415926536

float random(float seed){
	return fract(cos(seed/phase)*sin(seed/phase));
}
vec2 point(float seed){
	vec2 p=vec2(random(seed),random(seed+2.0));
	p.x+=mpos/random(seed/1000.0*sin(mpos*0.12))*sin(seed/112451.);
	p.y+=mpos/random(seed/2230.0*sin(mpos*0.12))*sin(seed/112451.);
	return vec2(mod(p.x*zoom,RENDERSIZE.x/RENDERSIZE.y/zoom),mod(p.y*zoom,RENDERSIZE.x/RENDERSIZE.y/zoom));
}

void main( void ) {

	vec2 position = ( gl_FragCoord.xy / RENDERSIZE.xy )*zoom - center*zoom;
  position.x*=(RENDERSIZE.x/RENDERSIZE.y)*zoom;
  position.y*=(RENDERSIZE.y/RENDERSIZE.x)*zoom;
	for(int i=1;i<100;i++){
		vec2 new=position;
		new.x+=(cos(position.y+mpos*0.3)*0.003/cos(mpos+sin(mpos*0.011))/float(i))*warp/zoom;
		new.y+=(cos(position.x+mpos*0.3)*0.003/cos(mpos+cos(mpos*0.011))/float(i))*warp/zoom;
		position=new;
	}
	
	float color=1.0;
	for(int i=1;i<30;i++){
		float d=distance(position-center,point(float(i)));
		if(d<color)color=d;
	}
	vec3 c=vec3(cos(color*PI*20.0+mpos),
		    cos(color*PI*40.0+mpos/2.0),
		    cos(color*PI*10.0+mpos/4.0));
	gl_FragColor = vec4( c, 1.0 );

}