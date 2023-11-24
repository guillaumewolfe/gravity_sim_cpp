// fragment_bloom.glsl
#version 120

// Entrée du vertex shader
varying vec2 TexCoords;

// Texture du framebuffer (image floue)
uniform sampler2D bloomTexture;

void main() {
    // Obtenez la couleur de l'image floue
    vec4 bloomColor = texture2D(bloomTexture, TexCoords);

    // Appliquez ici tout effet supplémentaire si nécessaire

    // Sortie la couleur finale
    gl_FragColor = bloomColor;
}
