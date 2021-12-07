#ifndef WARAXE_H
#define WARAXE_H
#include "Weapon.h"

using namespace MATH;

class WarAxe : public Weapon {

public:
	WarAxe(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_);
	WarAxe(int weaponID_);
};

#endif