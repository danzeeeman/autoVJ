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


#define pi 3.1415926536
#define N 12

vec4 main2( vec2 position) {
	vec2 center=position*2.-1.;
	center.x*=RENDERSIZE.x/RENDERSIZE.y;
	float c=0.;
	float r=0.3;
	float o;
	for(int i=0;i<N;i++)
	{
		vec2 xy;
		o=float(i)/float(N)*2.*pi;
		xy.x=r*cos(o);
		xy.y=r*sin(o);
		xy+=center;
		c+=pow(200000.,(1.-length(xy)*1.9)*(0.99+0.1*fract(float(-i)/float(N)-TIME*1.5)))/20000.0;
	}
	return vec4( c*vec3(0.1,.15,.2),1.0 );

}

vec4 main3( vec2 position) {
	vec2 center=position*2.-1.;
	center.x*=RENDERSIZE.x/RENDERSIZE.y;
	float c=0.;
	float r=0.5;
	float o;
	for(int i=0;i<N;i++)
	{
		vec2 xy;
		o=float(i)/float(N)*2.*pi;
		xy.x=r*cos(o);
		xy.y=r*sin(o);
		xy+=center;
		c+=pow(300000.,(1.-length(xy)*1.9)*(0.99+0.3*fract(float(-i)/float(N)-TIME*0.5)))/300000.0;
	}
	return vec4( sqrt(c*vec3(.6,.08,.03)),1.0 );
}

void main(void) {
	vec2 position = ( gl_FragCoord.xy / RENDERSIZE.xy );
	vec2 p2 = vec2(fract(position.x*2.0), fract(position.y*2.0));
	vec2 p3 = vec2(fract(position.x*3.0), fract(position.y*3.0));
	gl_FragColor = main2(p3) + main3(p3) + main2(p2) + main3(p2);
}