#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

out vec3 fragNormal;
out vec2 fragTexCoords;
out float fragLinearDepth;

float near = 0.1;
float far = 100.0;

float linearizeDepth(float depth) {
    return (2.0 * near * far) / (far + near - depth * (far - near));
}

float logisticDepth(float depth, float steepness, float offset) {
    float zVal = linearizeDepth(depth);
    return 1.0 / (1.0 + exp(-steepness * (zVal - offset)));
}

void main() {
    gl_Position = uMVPMatrix * vec4(inPosition, 1.0);
    fragNormal = (uNormalMatrix * vec4(inNormal, 1)).xyz;
    fragTexCoords = inTexCoords;

    // Output linearized depth value
    float depth = logisticDepth(gl_Position.z / gl_Position.w, 1.5, 1.0);
    fragLinearDepth = depth;
}
