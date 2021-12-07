#include "Axe.h"

Axe::Axe(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_) : Weapon(mesh, shader, texture, pos) {
	pos = pos_;
	mesh = mesh_;
	shader = shader_;
	texture = texture_;

	weaponID = 2;
	damageType = 1;
	swingDamage = 60;
	stabDamage = 10;
	delay = 1;
	reach = 2;
}

Axe::Axe(int weaponID_) {
	weaponID = weaponID_;
	damageType = 1;
	swingDamage = 60;
	stabDamage = 10;
	delay = 1;
	reach = 2;
}