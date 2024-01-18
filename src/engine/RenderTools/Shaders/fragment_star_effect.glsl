#version 120

uniform float alpha;
uniform vec3 sphereColor;

void main() {
    gl_FragColor = vec4(sphereColor, alpha);
}