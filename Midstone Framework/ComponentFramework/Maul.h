#ifndef MAUL_H
#define MAUL_H
#include "Weapon.h"

using namespace MATH;

class Maul : public Weapon {

public:
	Maul(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_);
	Maul(int weaponID_);
};

#endif