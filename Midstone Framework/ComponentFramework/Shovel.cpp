#include "Shovel.h"

Shovel::Shovel(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_): Weapon(mesh, shader, texture, pos){
	pos = pos_;
	mesh = mesh_;
	shader = shader_;
	texture = texture_;

	weaponID = 12;
	damageType = 3;
	swingDamage = 20;
	stabDamage = 20;
	delay = 2;
	reach = 4;
}