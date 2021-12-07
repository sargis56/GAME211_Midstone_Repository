#include "Maul.h"

Maul::Maul(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_) : Weapon(mesh, shader, texture, pos) {
	pos = pos_;
	mesh = mesh_;
	shader = shader_;
	texture = texture_;

	weaponID = 4;
	damageType = 3;
	swingDamage = 100;
	stabDamage = 10;
	delay = 4;
	reach = 4;
}

Maul::Maul(int weaponID_) {
	weaponID = weaponID_;
	damageType = 3;
	swingDamage = 100;
	stabDamage = 10;
	delay = 4;
	reach = 4;
}