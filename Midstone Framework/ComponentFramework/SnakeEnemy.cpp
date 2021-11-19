#include "SnakeEnemy.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>
#include "ObjLoader.h"
#include "Randomizer.h"

SnakeEnemy::SnakeEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_) :
	mesh(mesh_), shader(shader_), texture(texture_), room(room_) {
}

SnakeEnemy::~SnakeEnemy() {}



bool SnakeEnemy::OnCreate() {

	BuildVProjectile();
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
	//vProjectile->Render();
	/// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SnakeEnemy::HandleEvents(const SDL_Event& event) {
	
} /// Just a stub

bool SnakeEnemy::DamageCheck(Character* character) {
	if (VMath::distance(character->getPos(), pos) < 1) {
		return true;
	}
	else {
		return false;
	}
}

float SnakeEnemy::FollowPlayer(Character* character)
{
	PatrolArea();

	if (VMath::distance(character->getPos(), pos) < agroRange) {
		direction = character->getPos() - pos;
		float angle = atan2(direction.y, direction.x) * RADIANS_TO_DEGREES; //Calculate the angle (in DEGREES) between player and enemy
		this->setModelMatrix(this->getModelMatrix() * MMath::rotate(angle, Vec3(0.0f, 0.0f, 1.0f)) //Rotate the modelMatrix by the angle of rotation to face player
															* MMath::rotate(90.0f, Vec3(0.0f, 0.0f, 1.0f))); // Adjust enemy to FACE player 
		direction.Normalize();
		MoveEnemy();
		return angle;
	}
	else
	{
		return 0;
	}

}

void SnakeEnemy::PatrolArea()
{
	srand((unsigned)time(0));
	int randomNum;

	Vec3 base1 = Vec3(-4.0, -4.0, -15.0f);
	Vec3 base2 = Vec3(-4.0, 4.0, -15.0f);
	Vec3 base3 = Vec3(4.0, 4.0, -15.0f);
	Vec3 base4 = Vec3(4.0, -4.0, -15.0f);
	Vec3 base5 = Vec3(0.0, 0.0, -15.0f);

	randomNum = (rand() % 5) + 1;
	if (randomNum == 1)
	{
		this->setPos(base1);
	}
	if (randomNum == 2)
	{
		this->setPos(base2);
	}
	if (randomNum == 3)
	{
		this->setPos(base3);
	}
	if (randomNum == 4)
	{
		this->setPos(base4);
	}
	if (randomNum == 5)
	{
		this->setPos(base5);
	}
}


void SnakeEnemy::MoveEnemy() {
	float moveSpeed = 0.06;
	pos += (direction * moveSpeed); //Move the enemy in the direction of the player at X speed
}

void SnakeEnemy::AttackPlayer(Character* chtr) {

// 	if (DamageCheck(chtr))
// 	{
// 		
// 	}

	
	
}