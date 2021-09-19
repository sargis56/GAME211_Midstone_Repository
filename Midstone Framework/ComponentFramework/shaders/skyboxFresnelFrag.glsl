#version 460
out vec4 fragColor;

in vec3 vertNormal;
in vec3 eyeDir;

uniform samplerCube skyboxTexture;

void main() {
	//vec3 reflection = reflect(eyeDir, normalize(vertNormal));
	//vec3 I = normalize(position - eyeDir);
	normalize(eyeDir);
	float ratio = 1.00 / 1.52;
	vec3 refraction = refract(eyeDir, normalize(vertNormal), ratio);
	//refraction.x = -refraction.x;
	vec3 reflection = normalize(reflect(eyeDir, normalize(vertNormal)));
	reflection.x = -reflection.x;
	vec4 reflectRGB = texture(skyboxTexture, reflection);
	vec4 refractRGB = texture(skyboxTexture, refraction);
	//vec3 fresnel = max(reflection, refraction);
	//vec4 fresnelRgb = fresnel + reflection;
	fragColor = mix(refractRGB, reflectRGB, max(dot(eyeDir, vertNormal), 0.0f));
} 