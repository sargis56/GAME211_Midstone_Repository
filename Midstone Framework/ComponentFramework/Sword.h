#ifndef SWORD_H
#define SWORD_H
#include "Weapon.h"

using namespace MATH;

class Sword : public Weapon {

public:
	Sword(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_);
};

#endif