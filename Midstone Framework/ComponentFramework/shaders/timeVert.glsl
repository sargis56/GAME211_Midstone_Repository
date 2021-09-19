#version 460
layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;

out vec3 vertDir;
//out vec3 offsetFrag;
out vec4 rgb1Frag;
out vec4 rgb2Frag;

layout (location = 0) uniform mat4 projectionMatrix;
layout (location = 1) uniform mat4 viewMatrix;
layout (location = 2) uniform mat4 modelMatrix;
layout (location = 3) uniform mat3 normalMatrix;
//uniform float time;
uniform sampler3D noise;
uniform vec3 offset;
uniform vec4 rgb1;
uniform vec4 rgb2;

void main() {
	//vertNormal = mat3(transpose(inverse(
	rgb1Frag = rgb1;
	rgb2Frag = rgb2;
	//vertNormal = vVertex.xyz;
	vec3 vertPos = vec3(modelMatrix * vVertex);
	vertDir = normalize(vertPos);
	//eyeDir = -vertDir;
	vec3 vertex = vec3(vVertex);
	vec4 noiseVec = texture(noise, (vertex * 0.4) + (0.25 * offset));
	//offsetFrag = offset;
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * (vVertex + noiseVec); 
}