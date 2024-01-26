#version 120

varying vec3 Normal;
varying vec3 FragPos;

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;

void main() {
    Normal = normalize(vertexNormal);
    gl_Position = gl_ModelViewProjectionMatrix * vec4(vertexPosition, 1.0);
    gl_FrontColor = vec4((Normal + 1.0) / 2.0, 1.0);
}

