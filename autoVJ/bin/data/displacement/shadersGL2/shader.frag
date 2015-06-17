#version 120

uniform sampler2DRect tex0;

varying vec2 texCoordVarying;
varying vec3 normal;
void main()
{
    vec4 col = texture2DRect(tex0, texCoordVarying);
    col = vec4(vec3(col.xyz*abs(normal)+col.xyz), col.w);
    gl_FragColor = col;
}