#include "RatEnemy.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>

RatEnemy::RatEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_) :
	mesh(mesh_), shader(shader_), texture(texture_), room(room_) {
}

RatEnemy::~RatEnemy() {}

bool RatEnemy::OnCreate() { 


	return true; 
}
void RatEnemy::OnDestroy() {}				  /// Just a stub
void RatEnemy::Update(float deltaTime_) {

	

	if (pos.x > desiredPos.x - 0.01 && pos.x < desiredPos.x + 0.01) { //fix floating point precision errors
		pos.x = desiredPos.x;
	}
	if (pos.y > desiredPos.y - 0.01 && pos.y < desiredPos.y + 0.01) {//fix floating point precision errors
		pos.y = desiredPos.y;
	}
	if (pos.x == desiredPos.x && pos.y == desiredPos.y) { //Reached desired pos
		moveOver = true;
	}
	if (moveOver == true) { //Gen next pos

		desiredPos = pos;
		direction = rand() % 4;
		if (direction == 0) {
			desiredPos.x = pos.x + 3;
		}
		else if (direction == 1) {
			desiredPos.y = pos.y + 3;
		}
		else if (direction == 2) {
			desiredPos.x = pos.x - 3;
		}
		else {
			desiredPos.y = pos.y - 3;
		}
		moveOver = false;
	}
	if (pos != desiredPos) { //if the desired pos is not reached continue to move
		if (pos.x < desiredPos.x) {
			if (room.InsideCollisionPosX(Vec3(pos.x + 0.1f, pos.y, pos.z), 0)) { //Collision check
				pos.x = pos.x + 0.05f;
			}
			else { //If moving in a wall, gen new move
				moveOver = true;
			}
		}
		else if (pos.x > desiredPos.x) {
			if (room.InsideCollisionNegX(Vec3(pos.x - 0.1f, pos.y, pos.z), 0)) {
				pos.x = pos.x - 0.05f;
			}
			else {
				moveOver = true;
			}
		}
		else if (pos.y < desiredPos.y) {
			if (room.InsideCollisionPosY(Vec3(pos.x, pos.y + 0.1, pos.z), 0)) {
				pos.y = pos.y + 0.05f;
			}
			else {
				moveOver = true;
			}
		}
		else if (pos.y > desiredPos.y) {
			if (room.InsideCollisionNegY(Vec3(pos.x, pos.y - 0.1, pos.z), 0)) {
				pos.y = pos.y - 0.05f;
			}
			else {
				moveOver = true;
			}
		}
	}
	//printf("desired pos: %f %f %f\n", desiredPos.x, desiredPos.y, desiredPos.z);
}

void RatEnemy::Render() const {
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

void RatEnemy::HandleEvents(const SDL_Event& event) {
	
} /// Just a stub

bool RatEnemy::DamageCheck(Character* character) {
	if (character->getPos().x == pos.x && character->getPos().y == pos.y) { //fix floating point precision errors
		return true;
	}
	//if () {//fix floating point precision errors
	//	return true;
	//}
	return false;
}

bool RatEnemy::FollowPlayer(Character* character, RatEnemy* ratEnemy)
{
	if (VMath::distance(character->getPos(), ratEnemy->getPos()) < 5)
	{
		Vec3 moveEnemy;

		Vec3 direction = character->getPos() - ratEnemy->getPos();
		float angle = atan2(direction.y, direction.x) * RADIANS_TO_DEGREES; //Calculate the angle (in DEGREES) between player and enemy
		ratEnemy->setModelMatrix(ratEnemy->getModelMatrix() * MMath::rotate(angle, Vec3(0.0f, 0.0f, 1.0f)) //Rotate the modelMatrix by the angle of rotation to face player
															* MMath::rotate(90.0f, Vec3(0.0f, 0.0f, 1.0f))); // Adjust enemy to FACE player 

		direction.Normalize();
		ratEnemy->MoveEnemy(ratEnemy, direction);

		return true;
	}
	else
	{
		return false;
	}

}

void RatEnemy::MoveEnemy(RatEnemy* ratEnemy, Vec3 direction)
{
	float moveSpeed = 0.1;
	ratEnemy->setPos(ratEnemy->getPos() + (direction * moveSpeed)); //Move the enemy in the direction of the player at X speed

}

