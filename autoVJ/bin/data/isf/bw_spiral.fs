/*
{
  "CATEGORIES": [
    "Automatically Converted"
  ],
  "INPUTS": [
    
  ]
}
*/


/*
  Daily an hour GLSL sketch by @chimanaco 4/30

  References:
  http://wgld.org/d/glsl/g004.html
*/

#ifdef GL_ES
precision mediump float;
#endif


void main( void ) {
  vec2 p = (gl_FragCoord.xy * 2.0 - RENDERSIZE) / max(RENDERSIZE.x, RENDERSIZE.y);
   
  float u = abs(sin( (atan(p.y, p.x) - length(p) * 90. * (sin(TIME) / 5.)  + 5. + TIME) * 10.0 * 0.5) * 0.5 * sin(TIME)) + 0.15;
  float t = 0.01 / abs(u / 1.6  - length(p));         
  gl_FragColor = vec4(vec3(t), 1.0);
}
