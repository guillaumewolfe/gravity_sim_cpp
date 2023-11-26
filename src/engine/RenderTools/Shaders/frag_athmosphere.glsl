#version 120

varying vec3 Normal;
varying vec3 FragPos;
uniform float alpha;
uniform vec3 lightDirection; // Direction de la lumière

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightDirection);

    // Calcul du produit scalaire
    float dotProduct = dot(norm, lightDir);

    // Couleurs pour la diffusion de Rayleigh et le coucher de soleil
    vec3 baseBlue = vec3(0.0, 0.30, 1.0); // Bleu de base
    vec3 baseRed = vec3(1.0, 0.1, 0.0); // Rouge orangé de base

    // Mélange des couleurs de base avec du blanc pour les adoucir
    vec3 rayleighColor = mix(baseBlue, vec3(1.0, 1.0, 1.0), 0.25); // Bleu moins intense
    vec3 sunsetColor =baseRed; // Rouge moins intense

    // Adoucissement de la transition
    float edgeWidth = 0.1; // Largeur de la zone de transition
    float edgeCenter = 0.1; // Centre de la zone de transition
    float weight = 1.0 - smoothstep(edgeCenter - edgeWidth, edgeCenter + edgeWidth, dotProduct);

    vec3 finalColor = mix(rayleighColor, sunsetColor, weight);

    // Calcul de la couleur diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuseColor = diff * vec4(1.0, 1.0, 1.0, 1.0);

    // Couleur ambiante
    vec4 ambientColor = vec4(0.05, 0.05, 0.05, 1.0);

    // Combinaison des couleurs finale
    gl_FragColor = (vec4(finalColor, alpha/4) * diffuseColor * 1.5) + (vec4(finalColor, alpha/2) * ambientColor);
}
