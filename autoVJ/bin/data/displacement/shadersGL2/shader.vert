#version 120

uniform sampler2DRect tex0;

varying vec2 texCoordVarying;
varying vec3 normal;

void main()
{
    // get the texture coordinates
    texCoordVarying = gl_MultiTexCoord0.xy;

    // get the position of the vertex relative to the modelViewProjectionMatrix
    vec4 position = ftransform();
    vec4 modifiedPosition = position;
    
    // we need to scale up the values we get from the texture
    float scale = 250.0;
    
    // here we get the red channel value from the texture
    // to use it as vertical displacementd
//    float displacementX = texture2DRect(tex0, texCoordVarying).x;
    float displacementY = length(texture2DRect(tex0, texCoordVarying).xyz);
//    float displacementZ = texture2DRect(tex0, texCoordVarying).z;
    
    
    normal = (normalize(modifiedPosition.xyz - vec3(0, 0, 0)));
    modifiedPosition.y -= displacementY * scale*normal.y;
    modifiedPosition.x -= displacementY * scale*normal.x;
    modifiedPosition.z -= displacementY * scale*normal.z;
    
    

    // this is the resulting vertex position
	gl_Position = modifiedPosition;
}