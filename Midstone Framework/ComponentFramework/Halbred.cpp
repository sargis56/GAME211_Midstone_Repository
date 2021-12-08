#include "Halbred.h"

Halbred::Halbred(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_) : Weapon(mesh, shader, texture, pos) {
	pos = pos_;
	mesh = mesh_;
	shader = shader_;
	texture = texture_;

	weaponID = 7;
	damageType = 1;
	swingDamage = 80;
	stabDamage = 60;
	delay = 2.5;
	reach = 9;
}

Halbred::Halbred(int weaponID_) {
	weaponID = weaponID_;
	damageType = 1;
	swingDamage = 80;
	stabDamage = 60;
	delay = 2.5;
	reach = 9;
}