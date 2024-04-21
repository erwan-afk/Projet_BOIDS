#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;          // Normale du fragment
in vec2 fragTexCoords;       // Coordonnées de texture
in float fragLinearDepth;    // Profondeur linéaire

out vec4 fragColor;         // Couleur finale du fragment

uniform sampler2D textureSampler;  // Échantillonneur de texture
uniform vec3 colorFog;              // Couleur du brouillard

uniform vec3 lightPosition;         // Position de la lumière
uniform vec3 lightColor;            // Couleur de la lumière
uniform float lightIntensity;       // Intensité de la lumière
uniform float lightAttenuation;     // Atténuation de la lumière

void main() {
    // Échantillonnage de la texture en utilisant les coordonnées de texture
    vec4 texColor = texture(textureSampler, fragTexCoords);
    if(texColor.a < 0.1)
        discard;

    // Calcul de la direction de la lumière
    vec3 lightDirection = normalize(lightPosition - fragPosition);

    // Calcul de la distance entre le fragment et la lumière
    float distance = length(lightPosition - fragPosition);

    // Calcul de l'atténuation de la lumière
    float attenuation = 1.0 / (1.0 + lightAttenuation * pow(distance, 2));

    // Calcul de l'éclairage diffus
    float diff = max(dot(normalize(fragNormal), normalize(lightDirection)), 0.0);
    vec3 diffuse = lightColor * lightIntensity * diff * attenuation;

    // Application de l'effet de profondeur
    float depthEffect = fragLinearDepth;

    // Combinaison de la couleur de la texture avec l'effet de profondeur et l'éclairage diffus
    vec3 finalColor = texColor.rgb * (1.0 - depthEffect) + depthEffect * colorFog + diffuse;

    // Sortie de la couleur finale
    fragColor = vec4(finalColor, 1.0);
}
