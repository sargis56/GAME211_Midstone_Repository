#version 460
layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;

out vec3 vertNormal;
out vec3 lightDir[2];
out vec3 eyeDir; 
out float numLightFrag;
layout (location = 0) uniform mat4 projectionMatrix;
layout (location = 1) uniform mat4 viewMatrix;
layout (location = 2) uniform mat4 modelMatrix;
layout (location = 3) uniform mat3 normalMatrix;
layout (location = 4) uniform vec3 lightPos[2];
uniform float numLight;

void main() {
	vertNormal = normalMatrix * vNormal.xyz; /// Rotate the normal to the correct orientation 

	vec3 vertPos = vec3(viewMatrix * modelMatrix * vVertex); /// This is the position of the vertex from the origin
	vec3 vertDir = normalize(vertPos);
	eyeDir = -vertDir;
	for(int i = 0; i < numLight; i++){
	lightDir[i] = normalize(lightPos[i] - vertPos); /// Create the light direction. I do the math with in class 
	numLightFrag++;
	}
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vVertex; 
}
