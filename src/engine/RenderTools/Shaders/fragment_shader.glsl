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

    // Diffusion simple
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuseColor = diff * vec4(1.0, 1.0, 1.0, 1.0);

    vec4 ambientColor = vec4(0.15, 0.15, 0.15, 1.0);
    vec4 textureColor = texture2D(textureSampler, TexCoord);

    //gl_FragColor = (textureColor * diffuseColor * 1.5) + (textureColor * ambientColor);
    gl_FragColor = textureColor;
    //vec3 color = norm * 0.5 + 0.5;
    //gl_FragColor = vec4(color, 1.0); // Afficher la normale sous forme de couleur
    
}


