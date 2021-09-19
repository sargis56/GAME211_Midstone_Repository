#version 460
in  vec3 vertNormal;
in  vec3 lightDir;
in  vec3 eyeDir;
in  vec3 diffuseFrag;
in  vec3 specularFrag;
out vec4 fragColor;

void main() { 
	vec4 ks = vec4(specularFrag.x, specularFrag.y, specularFrag.z, 0.0);
	vec4 kd = vec4(diffuseFrag.x, diffuseFrag.y, diffuseFrag.z, 0.0); 
	vec4 ka = 0.1 * kd;

	float diff = max(dot(vertNormal, lightDir), 0.0);
	/// Reflection is based incedent which means a vector from the light source
	/// not the direction to the light source
	vec3 reflection = normalize(reflect(-lightDir, vertNormal));
	float spec = max(dot(eyeDir, reflection), 0.0);
	if(diff > 0.0){
		spec = pow(spec,14.0);
	}
	fragColor =  ka + (diff * kd) + (spec * ks);	
} 

