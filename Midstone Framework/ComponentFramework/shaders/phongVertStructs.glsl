#version 460
layout (location = 0) in  vec4 vVertex;
layout (location = 1) in  vec4 vNormal;

out vec3 vertNormal;
out vec3 lightDir;
out vec3 eyeDir; 
out vec3 diffuseFrag;
out vec3 specularFrag;
layout (location = 0) uniform mat4 projectionMatrix;
layout (location = 1) uniform mat4 viewMatrix;
layout (location = 2) uniform mat4 modelMatrix;
layout (location = 3) uniform mat3 normalMatrix;
//layout (location = 4) uniform vec3 lightPos;
uniform vec3 position;
uniform vec3 diffuse;
uniform vec3 specular;

void main() {
	vertNormal = normalize(normalMatrix * vNormal.xyz); /// Rotate the normal to the correct orientation 

	vec3 vertPos = vec3(viewMatrix * modelMatrix * vVertex); /// This is the position of the vertex from the origin
	vec3 vertDir = normalize(vertPos);
	eyeDir = -vertDir;
	lightDir = normalize(position +  -vertPos); /// Create the light direction. I do the math with in class 
	diffuseFrag = diffuse;
	specularFrag = specular;
	gl_Position =  projectionMatrix * viewMatrix * modelMatrix * vVertex; 
}
