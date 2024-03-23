#version 330 core

in vec3 fragNormal;
in vec2 fragTexCoords;
in float fragLinearDepth;

out vec4 fragColor;

uniform sampler2D textureSampler;

void main() {
    // Sample texture using texture coordinates
    vec4 texColor = texture(textureSampler, fragTexCoords);

    // Apply depth effect
    float depthEffect = fragLinearDepth;

    // Combine texture color with depth effect
    vec3 finalColor = texColor.rgb * (1.0 - depthEffect) + depthEffect * vec3(0.0, 0.639, 1.0);

    // Output final color
    fragColor = vec4(finalColor, 1.0);

}
