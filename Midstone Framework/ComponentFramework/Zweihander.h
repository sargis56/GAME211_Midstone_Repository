#ifndef ZWEIHANDER_H
#define ZWEIHANDER_H
#include "Weapon.h"

using namespace MATH;

class Zweihander : public Weapon {

public:
	Zweihander(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_);
	Zweihander(int weaponID_);
};

#endif