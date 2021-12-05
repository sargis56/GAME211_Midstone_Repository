#include "DemonEnemy.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>
#include "ObjLoader.h"

#define LOG(x) std::cout << x << std::endl; //debugging function

DemonEnemy::DemonEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_, int damage_) :
	mesh(mesh_), shader(shader_), texture(texture_), room(room_) {
	damage = damage_;
}

DemonEnemy::~DemonEnemy() {}

bool DemonEnemy::OnCreate() {
	return true;
}
void DemonEnemy::OnDestroy() {}				  /// Just a stub
void DemonEnemy::Update(float deltaTime_) {

}

void DemonEnemy::Render() const {

	Matrix3 normalMatrix = MMath::transpose(MMath::inverse(modelMatrix));
	glUniformMatrix4fv(shader->getUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
	glUniformMatrix3fv(shader->getUniformID("normalMatrix"), 1, GL_FALSE, normalMatrix);
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	}
	mesh->Render();
	/// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DemonEnemy::HandleEvents(const SDL_Event& event) {

} /// Just a stub

bool DemonEnemy::DamageCheck(Character* character) {
	if (VMath::distance(character->getPos(), pos) < 1) {
		return true;
	}
	else {
		return false;
	}
}

bool DemonEnemy::WeaponColCheck(Character* character) {
	if (VMath::distance(character->getPos(), pos) < (2 + character->getCurrentWeaponReach())) { //
		return true;
	}
	else {
		return false;
	}
}

float DemonEnemy::FollowPlayer(Character* character)
{
	if (VMath::distance(character->getPos(), pos) < agroRange) {
		direction = character->getPos() - pos;
		float angle = atan2(direction.y, direction.x) * RADIANS_TO_DEGREES; //Calculate the angle (in DEGREES) between player and enemy
		//this->setModelMatrix(this->getModelMatrix() * MMath::rotate(angle, Vec3(0.0f, 0.0f, 1.0f)) //Rotate the modelMatrix by the angle of rotation to face player
															//* MMath::rotate(90.0f, Vec3(0.0f, 0.0f, 1.0f))); // Adjust enemy to FACE player 
		direction.Normalize();
		MoveEnemy();
		AttackPlayer(character);
		return angle;
	}
	else
	{
		return 0;
	}

}

void DemonEnemy::MoveEnemy() {
	//float moveSpeed = 0.06;
	pos += (direction * moveSpeed); //Move the enemy in the direction of the player at X speed
}

void DemonEnemy::AttackPlayer(Character* chtr) {

	attackTarget = Vec3(chtr->getPos().x, chtr->getPos().y, 0);

	//printf("attackdir %f    %f    \n", attackTarget.x, attackTarget.y);


}