/*
{
  "CATEGORIES": [
    "Automatically Converted"
  ],
  "INPUTS": [
    
  ],
  "PASSES": [
    {
      "TARGET": "backbuffer"
    }
  ],
  "PERSISTENT_BUFFERS": [
    "backbuffer"
  ]
}
*/


// forked from http://fe0km.blog.fc2.com/blog-entry-96.html

#ifdef GL_ES
precision mediump float;
#endif
 
 
float light( vec2 p )
{
    const int nr_light = 12;
    float b = 0.0;
    for( int i=0; i<nr_light; ++i )
    {
        float t = TIME;
        float r = t+ 3.14159265358979*2.0* float(i) /float(nr_light);
        vec2 lp = vec2(0.5,0.5) +0.5*cos(t)*(abs(cos(t+float(i)))*0.3+0.7)*vec2( cos(r), sin(r) );
        float d = pow( 1.0 /( distance( p, lp )+1.0 ), 60.0 );
        b += d;
    }
    return b;
}
 
void main()
{
    vec2 op = gl_FragCoord.xy / RENDERSIZE.xy;
    vec2 p = op-vec2(0.5,0.5);
    float dist = length(p);
    float th = atan( p.y, p.x );
    p = vec2( th*2.0/3.1415926535, 0.1/dist );
    p = mod( p*5.0, 1.0 );
 
    float b = light( p );
    const float alpha = 0.05;
    vec3 new = vec3(clamp(b,0.0,1.0));
    gl_FragColor = vec4( vec3(b), 1.0 );
    //gl_FragColor = vec4( new*alpha + IMG_NORM_PIXEL(backbuffer,op).xyz*(1.0-alpha), 1.0 );
}