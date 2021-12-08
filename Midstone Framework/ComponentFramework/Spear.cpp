#include "Spear.h"

Spear::Spear(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_) : Weapon(mesh, shader, texture, pos) {
	pos = pos_;
	mesh = mesh_;
	shader = shader_;
	texture = texture_;

	weaponID = 8;
	damageType = 2;
	swingDamage = 10;
	stabDamage = 80;
	delay = 1.5;
	reach = 10;
}

Spear::Spear(int weaponID_) {
	weaponID = weaponID_;
	damageType = 2;
	swingDamage = 10;
	stabDamage = 80;
	delay = 1.5;
	reach = 10;
}

Spear::Spear() {
	weaponID = 8;
	damageType = 2;
	swingDamage = 10;
	stabDamage = 80;
	delay = 1.5;
	reach = 10;
}