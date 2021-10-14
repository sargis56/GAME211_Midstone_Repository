#include "CharacterVariables.h"

CharacterVariables::CharacterVariables() {
	position = Vec3(0.0f, 0.0f, 0.0f);
	health = 100.0f;
	speed = 0.1f;
}

CharacterVariables::CharacterVariables(Vec3 position_, float health_, float speed_) {
	position = position_;
	health = health_;
	speed = speed_;
}

CharacterVariables::~CharacterVariables() {
}
