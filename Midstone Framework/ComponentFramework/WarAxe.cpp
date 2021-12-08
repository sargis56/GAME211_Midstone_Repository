#include "WarAxe.h"

WarAxe::WarAxe(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_) : Weapon(mesh, shader, texture, pos) {
	pos = pos_;
	mesh = mesh_;
	shader = shader_;
	texture = texture_;

	weaponID = 6;
	damageType = 1;
	swingDamage = 80;
	stabDamage = 15;
	delay = 2;
	reach = 5;
}

WarAxe::WarAxe(int weaponID_) {
	weaponID = weaponID_;
	damageType = 1;
	swingDamage = 80;
	stabDamage = 15;
	delay = 2;
	reach = 5;
}