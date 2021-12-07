#ifndef HAMMER_H
#define HAMMER_H
#include "Weapon.h"

using namespace MATH;

class Hammer : public Weapon {

public:
	Hammer(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_);
	Hammer(int weaponID_);
};

#endif