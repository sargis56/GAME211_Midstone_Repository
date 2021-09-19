#version 460
in  vec3 vertNormal;
in  vec3 lightDir[2];
in  vec3 eyeDir;
in float numLightFrag;
out vec4 fragColor;

void main() { 
	vec4 ks = vec4(0.2, 0.2, 0.2, 0.0);
	vec4 kd[2] = vec4[] (vec4(0.4, 0.1, 0.1, 0.0), vec4(0.1,0.1,0.4,0.0));
	vec4 ka = 0.1 * kd[0];

	for(int i = 0; i < 2; i++){
		float diff = max(dot(vertNormal, lightDir[i]), 0.0);
		/// Reflection is based incedent which means a vector from the light source
		/// not the direction to the light source
		vec3 reflection = normalize(reflect(-lightDir[i], vertNormal));
		float spec = max(dot(eyeDir, reflection), 0.0);
		if(diff > 0.0){
			spec = pow(spec,14.0);
		}

		fragColor +=  ka + (diff * kd[i]) + (spec * ks);
	}
} 