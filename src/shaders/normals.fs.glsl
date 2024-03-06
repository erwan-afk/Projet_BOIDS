#version 330 core

in vec3 fragNormal;
in vec2 fragTexCoords;

out vec4 fragColor;

void main() {
    // Use normal as color for demonstration purposes
    vec3 normalizedNormal = normalize(fragNormal);
    vec3 color = 1. * normalizedNormal;

    // You can use texture coordinates for texture sampling here
    // Example: vec4 texColor = texture(sampler, fragTexCoords);

    fragColor = vec4(color, 1.0);
}
