/*
{
  "CATEGORIES": [
    "Automatically Converted"
  ],
  "INPUTS": [
    
  ]
}
*/


//Ring of HSL lights with a mysterious black circle overlay 
//based on http://tokyodemofest.jp/2014/7lines/index.html
//by @felixturner 

#ifdef GL_ES
precision mediump float;
#endif

const float dots = 40.; //number of point lights
const float brightness = 0.012;
const float radius = .25; //radius of light ring

vec3 hsv2rgb(vec3 c){
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * normalize(mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y));
}
		
void main( void ) {
    
    vec2 p=(gl_FragCoord.xy-.5*RENDERSIZE)/min(RENDERSIZE.x,RENDERSIZE.y);//
    vec3 c=vec3(0,0,.1); //background color

    float b = brightness * (sin(TIME*1.)/2. + .6); //modulate brightness over TIME
	
    for(float i=0.;i<dots; i++){
	    
	//get location of dot
        float x = radius*cos(2.*3.14*float(i)/dots);
        float y = radius*sin(2.*3.14*float(i)/dots);
        vec2 o = vec2(x,y);
	    
	//get color of dot based on its index in the circle + TIME
	vec3 dotCol = hsv2rgb(vec3((i + TIME*10.)/dots,1.,1.0));
	    
        //get brightness of this pixel based on distance to dot
	c += b/(length(p-o))*dotCol;
    }
	
    //black circle overlay	   
    float dist = distance(p , vec2(0));  
    c = mix(vec3(0), c, smoothstep(0.27, 0.28, dist));
	
    gl_FragColor = vec4(c,1);
}