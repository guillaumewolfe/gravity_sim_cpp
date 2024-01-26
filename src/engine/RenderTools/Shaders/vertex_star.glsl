#version 120

    varying vec3 Normal;
    varying vec2 TexCoord; // Variable pour passer les coordonnées de texture au fragment shader

    
    attribute vec3 vertexNormal;
    attribute vec2 texCoord; // Attribut d'entrée pour les coordonnées de texture



    void main() {
        Normal = normalize(vertexNormal);
        TexCoord = texCoord; // Passer les coordonnées de texture
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
        gl_FrontColor = vec4((Normal + 1.0) / 2.0, 1.0);
    }