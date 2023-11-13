#version 120

varying vec3 Normal;
varying vec3 FragPos;
varying vec2 TexCoord;

uniform sampler2D textureSampler;
uniform vec3 lightPosition;
uniform vec3 viewPosition; // Position de la caméra ou direction de la vue
uniform float specularStrength; // Intensité de la réflexion spéculaire
uniform float shininess; // Facteur de brillance



void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    vec3 viewDir = normalize(viewPosition - FragPos);

    // Diffusion simple
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuseColor = diff * vec4(1.0, 1.0, 1.0, 1.0);

    // Réflexion spéculaire
    vec3 reflectDir = reflect(-lightDir, norm); // Direction de la réflexion
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec4 specularColor = spec * specularStrength * vec4(1.0, 1.0, 1.0, 1.0);

    vec4 ambientColor = vec4(0.05, 0.05, 0.05, 1.0);
    vec4 textureColor = texture2D(textureSampler, TexCoord);

    gl_FragColor = (textureColor * diffuseColor * 1.5) + (textureColor * ambientColor) + specularColor;
}
