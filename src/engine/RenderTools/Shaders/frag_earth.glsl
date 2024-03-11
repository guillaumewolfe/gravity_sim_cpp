#version 120

varying vec3 Normal;
varying vec2 TexCoord;

uniform sampler2D dayTextureSampler;  // Sampler for the day texture
uniform sampler2D nightTextureSampler; // Sampler for the night texture
uniform vec3 lightDirection; // Direction of the light (sun)
uniform bool isThereSun;

void main() {
    vec3 norm = normalize(Normal);
    float angle = max(dot(norm, lightDirection), 0.0);
    // Calculate blend factor for smooth transition
    float blendFactor = smoothstep(0.0, 1.0, angle);

    // Sample both textures
    vec4 dayColor = texture2D(dayTextureSampler, TexCoord);
    vec4 nightColor = texture2D(nightTextureSampler, TexCoord);

    // Initial color based on day/night texture blend
    vec4 color = mix(nightColor, dayColor, blendFactor);

    // Enhanced ambient light for the night side
    vec4 ambientNight = vec4(0.75, 0.75, 0.75, 1.0);
    
    if (isThereSun) {
        // Apply diffuse lighting to day side
        vec4 diffuse = vec4(1.0, 1.0, 1.0, 1.0) * angle;
        // Blend diffuse lighting into day side and ambient into night side
        color = mix(color * ambientNight, color * (diffuse + ambientNight), blendFactor);
    } else {
        // When there is no sun, show only the night side with ambient lighting
        color = nightColor * ambientNight;
    }

    gl_FragColor = color;
}