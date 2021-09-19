#ifndef LIGHT_H
#define LIGHT_H
#include "Vector.h"
using namespace MATH;
class Light {
public:
	Vec3 position;
	Vec3 diffuse;
	Vec3 specular;
	Light();
	Light(Vec3 position_, Vec3 diffuse_, Vec3 specular_);
};
#endif