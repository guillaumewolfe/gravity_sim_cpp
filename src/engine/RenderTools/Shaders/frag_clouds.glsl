#version 120

varying vec3 Normal;
varying vec3 FragPos;
varying vec2 TexCoord;

uniform sampler2D textureSampler;
uniform vec3 lightDirection;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightDirection);
    float diff = max(dot(norm, lightDir), 0.0);
    
    vec4 diffuseColor = diff * vec4(1.0, 1.0, 1.0, 1.0);
    vec4 ambientColor = vec4(0.05, 0.05, 0.05, 1.0);
    vec4 textureColor = texture2D(textureSampler, TexCoord);

    // Vérifier si la couleur est proche du noir
    float threshold = 0.35; // Ajustez ce seuil si nécessaire
    bool isNearlyBlack = (textureColor.r < threshold) && (textureColor.g < threshold) && (textureColor.b < threshold);
    float alpha = isNearlyBlack ? 0.0 : 0.50;

    gl_FragColor = vec4(textureColor.rgb * diffuseColor.rgb * 1.1 + textureColor.rgb * ambientColor.rgb, alpha);
}