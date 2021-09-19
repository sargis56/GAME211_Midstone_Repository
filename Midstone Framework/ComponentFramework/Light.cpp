#include "Light.h"
Light::Light() {
	position.x = position.y = position.z = 0.0f;
	diffuse.x = diffuse.y = diffuse.z = 0.0f;
	specular.x = specular.y = specular.z = 0.0f;
}
Light::Light(Vec3 position_, Vec3 diffuse_, Vec3 specular_) {
	position = position_;
	diffuse = diffuse_;
	specular = specular_;
}