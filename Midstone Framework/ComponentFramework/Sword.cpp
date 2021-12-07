#include "Sword.h"

Sword::Sword(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_) : Weapon(mesh, shader, texture, pos) {
	pos = pos_;
	mesh = mesh_;
	shader = shader_;
	texture = texture_;

	weaponID = 1;
	damageType = 2;
	swingDamage = 40;
	stabDamage = 55;
	delay = 1;
	reach = 4;
}

Sword::Sword(int weaponID_) {
	weaponID = weaponID_;
	damageType = 2;
	swingDamage = 40;
	stabDamage = 55;
	delay = 1;
	reach = 4;
}

Sword::Sword() {
	weaponID = 1;
	damageType = 2;
	swingDamage = 40;
	stabDamage = 55;
	delay = 1;
	reach = 4;
}