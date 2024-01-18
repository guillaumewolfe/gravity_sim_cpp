#version 120

varying vec3 Normal;
varying vec2 TexCoord;

uniform sampler2D textureSampler;

void main() {
    vec4 textureColor = texture2D(textureSampler, TexCoord);

    // Calculer la distance par rapport au centre du soleil
    float distance = length(TexCoord - vec2(0.5, 0.5));
    
    // Ajuster l'intensité de l'éclat en fonction de la distance
    float glowIntensity = 1.0 - distance; // Plus lumineux au centre
    glowIntensity = clamp(glowIntensity, 0.0, 1.0); // S'assurer que l'intensité reste dans la plage [0, 1]

    // Couleur de l'éclat (ajuster selon le besoin)
    vec4 glowColor = vec4(1.0, 0.8, 0.5, 1.0); // Une teinte légèrement orange

    // Effet d'éclat plus réaliste en utilisant la fonction pow
    glowIntensity = pow(glowIntensity, 4.0); // Ajuster l'exposant selon le besoin pour un effet plus doux ou plus fort

    // Ajouter un effet de bord brillant
    float edgeGlow = 1.0 - smoothstep(0.45, 0.5, distance); // Ajuster les valeurs de smoothstep selon le besoin
    edgeGlow = clamp(edgeGlow, 0.0, 1.0); // S'assurer que l'intensité reste dans la plage [0, 1]

    // Combinez l'effet de bord brillant avec l'éclat principal
    float finalIntensity = glowIntensity + edgeGlow;
    finalIntensity = clamp(finalIntensity, 0.0, 1.0);

    // Mélanger la couleur de l'éclat avec la texture
    vec4 finalColor = mix(textureColor*1, glowColor, finalIntensity * 0.2); // Ajuster le facteur de mélange selon le besoin

    gl_FragColor = finalColor;
}
