#version 460
out vec4 fragColor;

in vec3 vertDir;
//in vec3 offsetFrag;
in vec4 rgb1Frag;
in vec4 rgb2Frag;
uniform vec3 offset;
uniform sampler3D noise;

void main() {
	vec4 noiseVec = texture(noise, (vertDir * 1.25) + offset);
    //vec4 rgb1 = vec4(rgb1Frag.x, rgb1Frag.y, rgb1Frag.z,0.0);
	//vec4 rgb2 = vec4(rgb2Frag.x, rgb2Frag.y, rgb2Frag.z,0.0);
	float intensity = abs(noiseVec[0] - 0.25) + abs(noiseVec[1] - 0.125) + abs(noiseVec[2] - 0.0625) + abs(noiseVec[3] - 0.03125);
	intensity = clamp(intensity * 4.0, 0.0, 1.0);
	fragColor = mix(rgb1Frag, rgb2Frag, intensity);
}