#version 460
in  vec3 vertNormal;
in  vec3 lightDir;
in  vec3 eyeDir;
in  vec2 uvCoords;
out vec4 fragColor;

uniform sampler2D objTexture;
void main() { 
	
	vec4 textureColor = texture(objTexture,uvCoords);
	float diff = max(dot(vertNormal, lightDir), 0.0);

	/// Reflection is based incedent which means a vector from the light source
	/// not the direction to the light source
	vec3 reflection = normalize(reflect(-lightDir, vertNormal));
	float spec = max(dot(eyeDir, reflection), 0.0);
	if(diff > 0.0){
		spec = pow(spec,14.0);
	}
	vec4 ka = 0.1 * textureColor;
	vec4 ks = vec4(0.2,0.2,0.2,0.0);
	fragColor =  ka + (diff * textureColor) + (spec * ks);	
} 