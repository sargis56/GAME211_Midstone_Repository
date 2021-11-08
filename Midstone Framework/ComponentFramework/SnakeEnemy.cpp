#include "SnakeEnemy.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>
#include "ObjLoader.h"

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

	
	/*if (vProjectile->VProjectileUpdate(attackTarget));
	{
		vProjectile->setPos(pos);
		vProjectile->setOver(false);
	}*/

	vProjectile->setModelMatrix(MMath::translate(vProjectile->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f));
	printf(" %f    %f    \n", vProjectile->getPos().x, vProjectile->getPos().y);

}

void SnakeEnemy::Render() const {
	
	Matrix3 normalMatrix = MMath::transpose(MMath::inverse(modelMatrix));
	glUniformMatrix4fv(shader->getUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
	glUniformMatrix3fv(shader->getUniformID("normalMatrix"), 1, GL_FALSE, normalMatrix);
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	}
	mesh->Render();
	vProjectile->Render();
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

void SnakeEnemy::BuildVProjectile() {
	ObjLoader::loadOBJ("meshes/CoronaVirus.obj");
	meshVProjectile = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderVProjectile = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	textureVProjectile = new Texture();
	textureVProjectile->LoadImage("textures/skull_texture.jpg");
	vProjectile = new VenomProjectile(meshVProjectile, shaderVProjectile, textureVProjectile, room, pos);
	Vec3 snakePos = this->getPos();
	vProjectile->setPos(snakePos);
}

void SnakeEnemy::MoveEnemy() {
	float moveSpeed = 0.06;
	pos += (direction * moveSpeed); //Move the enemy in the direction of the player at X speed
}

void SnakeEnemy::AttackPlayer(Character* chtr) {

		attackTarget = Vec3(chtr->getPos().x, chtr->getPos().y, 0);

		vProjectile->VProjectileUpdate(attackTarget);
		if (vProjectile->getPos().x && vProjectile->getPos().y == chtr->getPos().x && chtr->getPos().y)
		{
			vProjectile->setPos(pos);
			
		}


		
		//printf("attackdir %f    %f    \n", attackTarget.x, attackTarget.y);
	
	
}