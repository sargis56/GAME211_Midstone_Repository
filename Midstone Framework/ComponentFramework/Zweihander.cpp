#include "Zweihander.h"

Zweihander::Zweihander(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_) : Weapon(mesh, shader, texture, pos) {
	pos = pos_;
	mesh = mesh_;
	shader = shader_;
	texture = texture_;

	weaponID = 3;
	damageType = 2;
	swingDamage = 80;
	stabDamage = 60;
	delay = 3;
	reach = 9;

	altMode = true;
	altDamageType = 2;
	altSwingDamage = 60;
	altStabDamage = 80;
	altDelay = 0;
	altReach = 7;
}