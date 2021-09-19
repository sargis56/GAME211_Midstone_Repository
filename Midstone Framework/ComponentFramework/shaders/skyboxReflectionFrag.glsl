#version 460
out vec4 fragColor;

in vec3 vertNormal;
in vec3 eyeDir;

uniform samplerCube skyboxTexture;

void main() {
    //vec3 reflection = reflect(eyeDir, normalize(vertNormal));
    //vec3 I = normalize(position - eyeDir);
    normalize(eyeDir);
    vec3 reflection = normalize(reflect(eyeDir, normalize(vertNormal)));
    reflection.x = -reflection.x;
    fragColor = vec4(texture(skyboxTexture, reflection).rgb, 0.0);
}