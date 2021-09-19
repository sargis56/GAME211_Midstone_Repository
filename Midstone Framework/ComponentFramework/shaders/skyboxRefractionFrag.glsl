#version 460
out vec4 fragColor;

in vec3 vertNormal;
in vec3 eyeDir;

uniform samplerCube skyboxTexture;
//uniform mat4 viewMatrix;

void main() {
	//vec3 reflection = reflect(eyeDir, normalize(vertNormal));
	//vec3 I = normalize(position - eyeDir);
	normalize(eyeDir);
	float ratio = 1.00 / 1.52;
	vec3 refraction = refract(eyeDir, normalize(vertNormal), ratio);
	refraction.x = -refraction.x;
	//refraction.y = -refraction.y;
	//vec3 worldR = inverse(mat3(viewMatrix)) * refraction;
	fragColor = vec4(texture(skyboxTexture, refraction).rgb, 0.0);
} 