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



void main( void ) {
float t = TIME/1000.;
float px = gl_FragCoord.x/100. - 1.;
float py = 6.*tan(gl_FragCoord.y)/gl_FragCoord.y;
float sx = py * sin( abs(sin(gl_FragCoord.x/abs(sin(t)))) * px - 5000. * t/2.);
float dy = 1./  (abs(py  - sx) ) + (py - px)*100.0;
float r = (px + 0.5) * dy;
float g = abs(sin(t))*0.9 * dy;
float b = dy;


	


	gl_FragColor = vec4( vec3( r,g,b ), 1.0 );

}