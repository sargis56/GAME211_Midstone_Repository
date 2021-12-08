#ifndef SPEAR_H
#define SPEAR_H
#include "Weapon.h"

using namespace MATH;

class Spear : public Weapon {

public:

	Spear();
	Spear(int weaponID_);
	Spear(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_);

};

#endif