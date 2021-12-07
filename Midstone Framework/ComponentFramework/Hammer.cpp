#include "Hammer.h"

Hammer::Hammer(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_) : Weapon(mesh, shader, texture, pos) {
	pos = pos_;
	mesh = mesh_;
	shader = shader_;
	texture = texture_;

	weaponID = 5;
	damageType = 3;
	swingDamage = 80;
	stabDamage = 10;
	delay = 2;
	reach = 2;
}

Hammer::Hammer(int weaponID_) {
	weaponID = weaponID_;
	damageType = 3;
	swingDamage = 80;
	stabDamage = 10;
	delay = 2;
	reach = 2;
}