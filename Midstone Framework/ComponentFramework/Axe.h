#ifndef Axe_H
#define Axe_H
#include "Weapon.h"

using namespace MATH;

class Axe : public Weapon {

public:
	Axe(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_);
};

#endif