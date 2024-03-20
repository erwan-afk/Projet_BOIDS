#version 330 core

in vec3 fragNormal;
in vec2 fragTexCoords; // Utilisez le même nom que celui défini dans le vertex shader pour les coordonnées de texture

out vec4 fragColor;

uniform sampler2D textureSampler; // Utilisez le même nom que celui défini dans le vertex shader pour l'uniforme de texture

void main() {
    // Utilisez la fonction texture pour échantillonner la texture à partir des coordonnées de texture
    vec4 texColor = texture(textureSampler, fragTexCoords);

    // Utilisez la couleur de la texture comme couleur finale
    fragColor = texColor;
}
