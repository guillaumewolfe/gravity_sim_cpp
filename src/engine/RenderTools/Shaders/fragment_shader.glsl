#version 120

varying vec3 Normal;
varying vec3 FragPos;
varying vec2 TexCoord;

uniform sampler2D textureSampler;
uniform vec3 lightDirection; // Direction de la lumière

void main() {
    vec3 norm = normalize(Normal);

        vec3 lightDir = normalize(lightDirection); // Utilisation de la direction de la lumière
        float diff = max(dot(norm, lightDir), 0.0);
        vec4 diffuseColor = diff * vec4(1.0, 1.0, 1.0, 1.0);
        vec4 ambientColor = vec4(0.05,0.05,0.05, 1.0);
        vec4 textureColor = texture2D(textureSampler, TexCoord);
        gl_FragColor = (textureColor * diffuseColor * 1.1) + (textureColor * ambientColor);
}
