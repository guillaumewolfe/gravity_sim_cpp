#version 120

varying vec3 Normal;
varying vec3 FragPos;
varying vec2 TexCoord;

uniform sampler2D textureSampler;
uniform vec3 lightPosition; // Position de la source lumineuse ponctuelle


void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos); // Direction vers la lumière
    // Diffusion simple
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuseColor = diff * vec4(1.0, 1.0, 1.0, 1.0); // Couleur de la lumière diffusée
    vec3 color = norm * 0.5 + 0.5;

    vec4 ambientColor = vec4(0.15, 0.15, 0.15, 1.0);

    // Combinaison de la couleur de la texture, de la lumière diffusée et de l'atténuation
    vec4 textureColor = texture2D(textureSampler, TexCoord);
    gl_FragColor = (textureColor) * diffuseColor*1.5 + (textureColor * ambientColor);
    //gl_FragColor = vec4(color, 1.0);
}