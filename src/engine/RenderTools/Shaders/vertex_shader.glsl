#version 120

    varying vec3 Normal;
    varying vec3 FragPos;
    varying vec2 TexCoord; // Variable pour passer les coordonnées de texture au fragment shader

    
    attribute vec3 vertexNormal;
    attribute vec2 texCoord; // Attribut d'entrée pour les coordonnées de texture
    uniform mat3 normalMatrix;

    void main() {
        //FragPos = gl_Vertex.xyz;
        Normal = normalize(normalMatrix * vertexNormal);
        TexCoord = texCoord; // Passer les coordonnées de texture
        FragPos = vec3(gl_ModelViewMatrix * gl_Vertex);
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
        gl_FrontColor = vec4((Normal + 1.0) / 2.0, 1.0);
    }