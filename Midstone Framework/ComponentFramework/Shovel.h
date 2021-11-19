#ifndef Shovel_H
#define Shovel_H
#include "Weapon.h"

using namespace MATH;

class Shovel : public Weapon {

public:
	Shovel(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_);
};

#endif