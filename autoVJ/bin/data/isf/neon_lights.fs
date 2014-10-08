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
	vec2 rminus;
	float res_y;
	if (RENDERSIZE.x > RENDERSIZE.y) {
		rminus = vec2(RENDERSIZE.x / RENDERSIZE.y, 1.0);
		res_y = RENDERSIZE.y;
	} else {
		rminus = vec2(1.0, RENDERSIZE.y / RENDERSIZE.x);
		res_y = RENDERSIZE.x;
	}

	vec2 position = gl_FragCoord.xy / res_y * 2.0 - rminus;
	
	vec3 col = vec3(0, 0, 0);
	
	float bt = TIME * 0.03;
	float broken = 1.1 - sin(bt) * sin(bt) * 0.35;
	float intensity = min(1.0, cos(bt) / 1.5 + 1.0);
		
	for (int i = 0; i < 7; ++i) {
		float mt = mod(TIME, 999.0) * 0.3 * (float(i) * 0.3);
		float t = mod(TIME * 0.4, 1111.0) * (float(i) + 2.0);
		float x2 = cos(t) * 0.7 + sin(mt) * 0.02;
		float y2 = sin(t) * 0.7 + cos(mt) * 0.02;
		float x = cos(t/0.3) * x2 + sin(t/0.3) * y2;
		//float y = sin(t/0.3) * x2 + cos(t/0.3) * y2;
		x = x2;
		float y = y2;
		vec2 d3 = position - vec2(x, y);
		
		float tt = t * (float(i) + 5.0) / 20.0 + d3.x * tan(t*1.5) + d3.y * tan(t*2.0);
		vec2 d = vec2(d3.x * cos(tt) + d3.y * sin(tt), d3.x * sin(tt) - d3.y * cos(tt));
		
		float rx = broken - (abs(sin(t * 0.5)) / 2.0);
		float ry = broken - (abs(cos(t * 0.5)) / 2.0);
		d.x *= rx;
		d.y *= ry;
		
		float d2 = dot(d, d);
		float a = abs(d2 - 0.03);

		float u = mod(TIME*2.0, 1111.0) * (float(i) * 0.1 + 2.0);
		col.r += 0.005 * sin(u) * sin(u) / (a + 0.00001);
		if (d2 < 0.03)
			col.r +=  0.3 * sin(u) * sin(u);

		u = mod(TIME*1.9, 1111.0) * (float(i) * 0.1 + 2.0);
		col.g += 0.005 * cos(u) * cos(u) / (a + 0.00001);
		if (d2 < 0.03)
			col.g +=  0.3 * sin(u) * sin(u);

		u = mod(TIME*1.8, 1111.0) * (float(i) * 0.1 + 2.0);
		col.b += 0.005 * sin(u+0.787) * sin(u+0.787) / (a + 0.00001);
		if (d2 < 0.03)
			col.b +=  0.3 * sin(u) * sin(u);
	}
	gl_FragColor = vec4(col * intensity, 1.0);
}