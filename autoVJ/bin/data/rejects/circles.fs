/*
{
  "CATEGORIES": [
    "Automatically Converted"
  ],
  "INPUTS": [
    
  ]
}
*/


//by @Flexi23, @DanielPettersso, @mrdoob

#ifdef GL_ES
precision highp float;
#endif
#define pi2_inv 0.159154943091895335768883763372

float border(vec2 uv, float thickness){
	uv = fract(uv - vec2(0.1));
	uv = min(uv, vec2(1.)-uv)*2.;
	return clamp(max(uv.x,uv.y)-1.+thickness,0.,1.)/thickness;
}

// complex multiplication
vec2 mul(vec2 a, vec2 b){
   return vec2( a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

vec2 div(vec2 numerator, vec2 denominator){
   return vec2( numerator.x*denominator.x + numerator.y*denominator.y,
                numerator.y*denominator.x - numerator.x*denominator.y)/
          vec2(denominator.x*denominator.x + denominator.y*denominator.y);
}

vec2 spiralzoom(vec2 domain, vec2 center, float n, float spiral_factor, float zoom_factor, vec2 pos){
	vec2 uv = domain - center;
	float d = length(uv);
	return vec2( atan(uv.y, uv.x)*n*pi2_inv + log(d)*spiral_factor, -log(d)*zoom_factor) + pos;
}

void main( void ) {
	vec2 uv = gl_FragCoord.xy / RENDERSIZE.xy;
	
	vec2 p1 = vec2(0.25+sin(TIME*.4)*.1, 0.3+cos(TIME*.4)*.1);
	vec2 p2 = vec2(0.75+sin(TIME)*.1, 0.7+cos(TIME)*.1);

	vec2 moebius = div(uv-p2, uv-p1);

	vec2 spiral_uv = spiralzoom(moebius,vec2(-0.1),1.,1.6,1.8,-vec2(0.5,0.5)*TIME*.3);
	vec2 spiral_uv2 = spiralzoom(moebius,vec2(-0.1),1.,1.9,1.2,-vec2(0.5,0.5)*TIME*.33);
	vec2 spiral_uv3 = spiralzoom(moebius,vec2(-0.1),1.,1.8,1.6,-vec2(0.5,0.5)*TIME*.38);

	gl_FragColor = vec4(border(spiral_uv,0.3), border(spiral_uv2,0.1) ,border(spiral_uv3,0.9),1.);

}