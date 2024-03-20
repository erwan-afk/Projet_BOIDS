#version 330 core

in vec3 fragNormal;
uniform sampler2D uTexture;
in vec2 vfragTextureCoord;
out vec4 fragColor;

void main() {
    vec4 tex = texture(uTexture, vfragTextureCoord);
    // Use normal as color for demonstration purposes
    vec3 normalizedNormal = normalize(fragNormal);
    vec3 color = 1. * normalizedNormal;

    // You can use texture coordinates for texture sampling here
    // Example: vec4 texColor = texture(sampler, fragTexCoords);

    fragColor = vec4(color, 1.0);
}
