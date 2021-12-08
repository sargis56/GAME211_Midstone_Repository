#ifndef Halbred_H
#define Halbred_H
#include "Weapon.h"

using namespace MATH;

class Halbred : public Weapon {

public:
	Halbred(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_);
	Halbred(int weaponID_);
};

#endif