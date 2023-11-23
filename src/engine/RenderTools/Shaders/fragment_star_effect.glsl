#version 120

uniform vec3 centerPosition;
uniform float maxGlowDistance;
uniform vec3 glowColor;

varying vec3 worldPosition;

void main()
{
    float distance = length(worldPosition - centerPosition);
    float glowFactor = 1.0 - clamp(distance / maxGlowDistance, 0.0, 1.0);
    vec4 color = vec4(glowColor * glowFactor, glowFactor);
    color = vec4(1,1,1,1)
    gl_FragColor = color;
}
