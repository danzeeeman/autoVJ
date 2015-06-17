#version 120

uniform sampler2DRect tex0;

varying vec2 texCoordVarying;

void main()
{
    gl_FragColor = vec4(1, 1, 0, 1);//texture2DRect(tex0, texCoordVarying);
}