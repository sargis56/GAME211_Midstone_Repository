#include "SnakeEnemy.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>

SnakeEnemy::SnakeEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_) :
	mesh(mesh_), shader(shader_), texture(texture_), room(room_) {
}

SnakeEnemy::~SnakeEnemy() {}

bool SnakeEnemy::OnCreate() {
	return true; 
}
void SnakeEnemy::OnDestroy() {}				  /// Just a stub
void SnakeEnemy::Update(float deltaTime_) {
	
}

void SnakeEnemy::Render() const {
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

void SnakeEnemy::HandleEvents(const SDL_Event& event) {
	
} /// Just a stub

bool SnakeEnemy::DamageCheck(Character* character) {
	if (character->getPos().x == pos.x && character->getPos().y == pos.y) { //fix floating point precision errors
		return true;
	}
	//if () {//fix floating point precision errors
	//	return true;
	//}
	return false;
}

bool SnakeEnemy::FollowPlayer(Character* character)
{
	if (VMath::distance(character->getPos(), pos) < 6) {
		direction = character->getPos() - pos;
		float angle = atan2(direction.y, direction.x) * RADIANS_TO_DEGREES; //Calculate the angle (in DEGREES) between player and enemy
		this->setModelMatrix(this->getModelMatrix() * MMath::rotate(angle, Vec3(0.0f, 0.0f, 1.0f)) //Rotate the modelMatrix by the angle of rotation to face player
															* MMath::rotate(90.0f, Vec3(0.0f, 0.0f, 1.0f))); // Adjust enemy to FACE player 
		direction.Normalize();
		MoveEnemy();

		return true;
	}
	else
	{
		return false;
	}

}

void SnakeEnemy::MoveEnemy()
{
	float moveSpeed = 0.06;
	pos = pos + (direction * moveSpeed); //Move the enemy in the direction of the player at X speed

}

