#version 460
layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;

//out vec3 position;
out vec3 vertNormal;
out vec3 eyeDir;

layout (location = 0) uniform mat4 projectionMatrix;
layout (location = 1) uniform mat4 viewMatrix;
layout (location = 2) uniform mat4 modelMatrix;
layout (location = 3) uniform mat3 normalMatrix;

void main() {
	//vertNormal = mat3(transpose(inverse(
	vertNormal = normalize(normalMatrix * vNormal.xyz);
	vec3 vertPos = vec3(inverse(viewMatrix) * modelMatrix * vVertex);
	vec3 vertDir = normalize(vertPos);
	eyeDir = -vertDir;
	gl_Position =  (projectionMatrix * viewMatrix) * modelMatrix * vVertex; 
}