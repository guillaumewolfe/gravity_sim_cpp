#version 120

varying vec3 Normal;
varying vec3 FragPos;
varying vec2 TexCoord;

uniform sampler2D textureSampler;
uniform vec3 lightDirection;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightDirection);

    // Calcul du produit scalaire
    float dotProduct = dot(norm, lightDir);
    // Adoucir la transition
    float smoothStepDot = smoothstep(-0.2, 1.0, dotProduct); // Ajustez les valeurs selon le besoin

    // Couleur diffuse plus douce
    vec4 diffuseColor = smoothStepDot * vec4(1.0, 1.0, 1.0, 1.0);
    vec4 ambientColor = vec4(0.05, 0.05, 0.05, 1.0);
    vec4 textureColor = texture2D(textureSampler, TexCoord);

    // Calcul de l'alpha avec lissage
    float alpha = mix(0.3, textureColor.a, smoothStepDot); // Mix entre le côté ombragé et le côté éclairé

    // Vérifier si la couleur est proche du noir
    float threshold = 0.35;
    bool isNearlyBlack = (textureColor.r < threshold) && (textureColor.g < threshold) && (textureColor.b < threshold);
    float alphaBlack = isNearlyBlack ? alpha*0.0 : alpha*0.5;

    // Mélange des couleurs avec le facteur alpha ajusté
    vec4 finalColor = vec4(mix(ambientColor.rgb, diffuseColor.rgb, smoothStepDot) * textureColor.rgb, alphaBlack);

    gl_FragColor = finalColor;
}
