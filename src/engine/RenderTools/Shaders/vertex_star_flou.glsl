#version 120

attribute vec3 vertexPosition;
attribute vec2 texCoord;

varying vec2 TexCoord;

void main() {
    TexCoord = texCoord;
    gl_Position = vec4(vertexPosition, 1.0);
}
