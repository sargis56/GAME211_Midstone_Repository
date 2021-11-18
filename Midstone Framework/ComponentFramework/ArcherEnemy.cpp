#include "ArcherEnemy.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>
#include "ObjLoader.h"

ArcherEnemy::ArcherEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_, Character* character_) :
	mesh(mesh_), shader(shader_), texture(texture_), room(room_), character(character_) {
}

ArcherEnemy::~ArcherEnemy() {}

void ArcherEnemy::BuildProjectile() {
	ObjLoader::loadOBJ("meshes/Sphere.obj");
	meshProjectile = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderProjectile = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	textureProjectile = new Texture();
	textureProjectile->LoadImage("textures/skull_texture.jpg");
	projectile = new ProjectileDynamic(meshProjectile, shaderProjectile, textureProjectile, room, pos, 10);
	projectileDestination = character->getPos() - pos;
	projectileDestination = VMath::normalize(Vec3(projectileDestination.x, projectileDestination.y, 1));
}

bool ArcherEnemy::OnCreate() {
	BuildProjectile();
	//GenerateProjectileDirection();
	return true; 
}
void ArcherEnemy::OnDestroy() {}				  /// Just a stub
void ArcherEnemy::Update(float deltaTime_) {
	Vec3 normalTowardsPlayer = character->getPos() - pos;
	normalTowardsPlayer = VMath::normalize(Vec3(normalTowardsPlayer.x, normalTowardsPlayer.y, 1));
	vel = Vec3(-normalTowardsPlayer.x, -normalTowardsPlayer.y, 0);
	if (room.InsideCollisionPosX(Vec3(pos.x + 0.1f, pos.y, pos.z), 0) == false) { //Collision check
		//vel = Vec3(0, -normalTowardsPlayer.y , 0);
		vel = Vec3(0, 0, 0);
	}
	else if (room.InsideCollisionNegX(Vec3(pos.x - 0.1f, pos.y, pos.z), 0) == false) { //Collision check
		//vel = Vec3(0, -normalTowardsPlayer.y, 0);
		vel = Vec3(0, 0, 0);
	}
	else if (room.InsideCollisionPosY(Vec3(pos.x, pos.y + 0.1, pos.z), 0) == false) { //Collision check
		//vel = Vec3(-normalTowardsPlayer.x, 0, 0);
		vel = Vec3(0, 0, 0);
	}
	else if (room.InsideCollisionNegY(Vec3(pos.x, pos.y - 0.1, pos.z), 0) == false) { //Collision check
		//vel = Vec3(-normalTowardsPlayer.x, 0, 0);
		vel = Vec3(0, 0, 0);
	}
	Physics::SimpleNewtonMotion(*this, 0.01);
	if (projectile->ProjectileDynamicUpdate(projectileDestination, 0.2)){ // so once it hits the wall
		projectile->setPos(pos); //the pos is reset to the pos of the enemy
		projectile->setOver(false); //and the moveOver is reset
		projectileDestination = normalTowardsPlayer;
		//projectileDestination = Vec3(character->getPos().x, character->getPos().y, character->getPos().z);
	}
	
	projectile->setModelMatrix(MMath::translate(projectile->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f)); //setting modelmatrix of the projectile
}

void ArcherEnemy::Render() const {
	projectile->Render();
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

void ArcherEnemy::HandleEvents(const SDL_Event& event) {
	
} /// Just a stub

bool ArcherEnemy::DamageCheck(Character* character) { //function to detect dmg
	bool damageTrue = false;
	if (projectile->DamageCheck(character)) {//check dmg of projectile
		damageTrue = true;
	}
	if (VMath::distance(character->getPos(), pos) < 1) { //if the enemy is overlaping the player
		damageTrue = true;
	}
	return damageTrue;
}