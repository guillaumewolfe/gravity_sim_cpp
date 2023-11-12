#version 120

varying vec3 Normal;
varying vec3 FragPos;
varying vec2 TexCoord;

uniform sampler2D textureSampler;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(0, 1, 1)); // Exemple de direction de lumière
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuseColor = diff * vec4(1.0, 1.0, 1.0, 1.0); // Lumière blanche

    vec4 textureColor = texture2D(textureSampler, TexCoord); // Couleur de la texture
    gl_FragColor = textureColor * diffuseColor;
}