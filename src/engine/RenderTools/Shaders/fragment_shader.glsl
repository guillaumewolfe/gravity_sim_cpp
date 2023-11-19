#version 120

varying vec3 Normal;
varying vec3 FragPos;
varying vec2 TexCoord;

uniform sampler2D textureSampler;
uniform vec3 lightDirection; // Direction de la lumière

void main() {
    vec3 norm = normalize(Normal);

    // Utilisez la composante Y de la normale pour déterminer si c'est le "dessus"
    /*if (norm.y > 0.85) {
        gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Vert pour +Y
    } else if (norm.z > 0.85) {
        gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0); // Bleu pour +Z
    } else if (norm.x > 0.85) {
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Rouge pour +X
    } else {
        // Calcul de l'éclairage
        vec3 lightDir = normalize(lightDirection); // Utilisation de la direction de la lumière
        float diff = max(dot(norm, lightDir), 0.0);
        vec4 diffuseColor = diff * vec4(1.0, 1.0, 1.0, 1.0);
        vec4 ambientColor = vec4(0.15, 0.15, 0.15, 1.0);
        vec4 textureColor = texture2D(textureSampler, TexCoord);
        gl_FragColor = (textureColor * diffuseColor * 1.5) + (textureColor * ambientColor);
    }*/
        vec3 lightDir = normalize(lightDirection); // Utilisation de la direction de la lumière
        float diff = max(dot(norm, lightDir), 0.0);
        vec4 diffuseColor = diff * vec4(1.0, 1.0, 1.0, 1.0);
        vec4 ambientColor = vec4(0.15, 0.15, 0.15, 1.0);
        vec4 textureColor = texture2D(textureSampler, TexCoord);
        //gl_FragColor = (textureColor * diffuseColor * 1.5) + (textureColor * ambientColor);
        gl_FragColor = textureColor;
}
