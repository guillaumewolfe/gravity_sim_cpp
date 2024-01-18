#version 120

varying vec3 Normal;
varying vec3 FragPos;

uniform vec3 lightDirection;
uniform vec3 sphereColor;
uniform float alpha;
uniform vec3 saturnPosition; // Position de Saturne
uniform float saturnSize;    // Une approximation de la taille de Saturne pour calculer l'ombre

void main() {
    float diff = max(dot(normalize(Normal), normalize(lightDirection)), 0.0);
    vec3 diffuse = diff * sphereColor;

    // Calculer la direction vers le centre de Saturne
    vec3 dirToSaturn = normalize(saturnPosition - FragPos);

    // Calculer si la particule est derrière Saturne par rapport à la lumière
    float shadow = step(saturnSize, dot(dirToSaturn, lightDirection));

    // Assombrir les particules derrière Saturne
    diffuse *= (1.0 - shadow * 0.5);
    vec3 ambient = 0.2 * sphereColor; // Éclairage ambiant pour éclaircir les zones ombrées
    vec3 color = ambient + diffuse;
    gl_FragColor = vec4(color, alpha);

}
