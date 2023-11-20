#version 120

varying vec3 Normal;
varying vec2 TexCoord;

uniform sampler2D textureSampler;

void main() {
    vec4 textureColor = texture2D(textureSampler, TexCoord);

    // Augmenter la luminosité de la texture
    vec4 brightColor = textureColor * 2; // Multiplier la couleur de la texture pour augmenter la luminosité

    // Ajouter un effet de glow
    vec4 glowColor = vec4(1.0, 1.0, 0.8, 1.0); // Couleur de l'éclat (ajustez selon vos besoins)
    vec4 finalColor = mix(brightColor, glowColor, 0.3); // Mélangez avec la couleur de glow

    gl_FragColor = finalColor;
}
