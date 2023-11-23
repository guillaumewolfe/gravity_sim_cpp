#version 120

attribute vec3 aPos;
uniform mat4 modelMatrix;

varying vec3 fragPosition;

void main()
{
    gl_Position = modelMatrix * vec4(aPos, 1.0);
    fragPosition = gl_Position.xyz;
}
