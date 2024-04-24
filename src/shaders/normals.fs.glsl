#version 330 core

in vec3 fragPosition;
in vec3 fragNormal;          // Normale du fragment
in vec2 fragTexCoords;       // Coordonnées de texture
in float fragLinearDepth;    // Profondeur linéaire

out vec4 fragColor;         // Couleur finale du fragment

uniform sampler2D textureSampler;  // Échantillonneur de texture
uniform vec3 colorFog;              // Couleur du brouillard

// Structure pour une lumière
struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    float radius;
};

// Uniforms pour les lumières
uniform int uNumLights;
uniform Light uLights[2];

void main() {
    // Échantillonnage de la texture en utilisant les coordonnées de texture
    vec4 texColor = texture(textureSampler, fragTexCoords);
    if(texColor.a < 0.1)
        discard;

    // Initialisation de la couleur finale
    vec3 finalColor = vec3(0.0);

    // Loop à travers les lumières
    for(int i = 0; i < uNumLights; ++i) {
        // Extraction des données de la lumière actuelle
        vec3 lightColor = uLights[i].color;
        float lightIntensity = uLights[i].intensity;

        // Calcul du vecteur de la lumière au fragment
        vec3 lightDirection = normalize(uLights[i].position - fragPosition);
        float distanceToLight = length(uLights[i].position - fragPosition);

        // Si la distance au fragment est supérieure au rayon de la sphère de lumière, l'atténuation est 0
        float attenuation = 0.0;
        if(distanceToLight <= uLights[i].radius) {
    // Calcul de l'atténuation de la lumière basée sur la loi de l'inverse du carré de la distance
            attenuation = 1.0 - (distanceToLight / uLights[i].radius); // Vous pouvez ajuster cette formule selon vos besoins
        }

        float diff = max(dot(normalize(fragNormal), lightDirection), 0.0);
        vec3 diffuse = lightColor * lightIntensity * diff * attenuation;

        // Ajout de l'éclairage diffus à la couleur finale
        finalColor += diffuse;
    }

    // Application de l'effet de profondeur
    float depthEffect = fragLinearDepth;

    // Combinaison de la couleur de la texture avec l'effet de profondeur, l'éclairage diffus et le brouillard
    finalColor = (texColor.rgb * finalColor) * (1.0 - depthEffect) + depthEffect * colorFog;

    // Sortie de la couleur finale
    fragColor = vec4(finalColor, 1.0);
}
