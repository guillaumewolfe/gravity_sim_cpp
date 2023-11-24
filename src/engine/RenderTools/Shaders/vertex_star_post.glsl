// vertex_bloom.glsl
#version 120

// Attributs d'entrée
attribute vec2 position; // Position du vertex
attribute vec2 texCoords; // Coordonnées de texture

// Sortie vers le fragment shader
varying vec2 TexCoords;

void main() {
    TexCoords = texCoords;
    gl_Position = vec4(position, 0.0, 1.0);
}
