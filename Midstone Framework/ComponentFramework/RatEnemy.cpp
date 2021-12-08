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
	if (textureChange == true) {
		textureChangeTimer++;
		//printf("%i\n", textureChangeTimer);
	}
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
				angle = 90;
				pos.x = pos.x + 0.05f;
			}
			else { //If moving in a wall, gen new move
				moveOver = true;
			}
		}
		else if (pos.x > desiredPos.x) {
			if (room.InsideCollisionNegX(Vec3(pos.x - 0.1f, pos.y, pos.z), 0)) {
				angle = 270;
				pos.x = pos.x - 0.05f;
			}
			else {
				moveOver = true;
			}
		}
		else if (pos.y < desiredPos.y) {
			if (room.InsideCollisionPosY(Vec3(pos.x, pos.y + 0.1, pos.z), 0)) {
				angle = 180;
				pos.y = pos.y + 0.05f;
			}
			else {
				moveOver = true;
			}
		}
		else if (pos.y > desiredPos.y) {
			if (room.InsideCollisionNegY(Vec3(pos.x, pos.y - 0.1, pos.z), 0)) {
				angle = 0;
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
	if (VMath::distance(character->getPos(), pos) < 2) {
		return true;
	}
	else {
		return false;
	}
}

bool RatEnemy::WeaponColCheck(Character* character) {
	if (VMath::distance(character->getPos(), pos) < (2 + character->getCurrentWeaponReach())) { //
		return true;
	}
	else {
		return false;
	}
}
void RatEnemy::TakeDamage(float damage) {
	health = health - damage;
	textureChange = true;
	//printf("%f", health);
	if (health <= 0) {
		isDead = false;
	}
}

void RatEnemy::ResetTimer() {
	textureChangeTimer = 0;
	textureChange = false;
}