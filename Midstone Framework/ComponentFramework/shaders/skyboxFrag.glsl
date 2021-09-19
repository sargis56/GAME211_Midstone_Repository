#version 460
out vec4 fragColor;

in vec3 uvwCoords;

uniform samplerCube skyboxTexture;
void main() {
	fragColor = texture(skyboxTexture,uvwCoords);
} 