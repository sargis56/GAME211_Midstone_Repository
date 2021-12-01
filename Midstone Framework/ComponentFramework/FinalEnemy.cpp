#include "FinalEnemy.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>
#include "ObjLoader.h"

FinalEnemy::FinalEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_, Character* character_) :
	mesh(mesh_), shader(shader_), texture(texture_), room(room_), character(character_) {
}

FinalEnemy::~FinalEnemy() {}

void FinalEnemy::BuildProjectile() {
	ObjLoader::loadOBJ("meshes/Sphere.obj");
	meshProjectile = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderProjectile = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	textureProjectile = new Texture();
	textureProjectile->LoadImage("textures/skull_texture.jpg");
	projectile1 = new ProjectileDynamic(meshProjectile, shaderProjectile, textureProjectile, room, pos, 3, 2.0f);
	projectile2 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos, 5);
	projectile3 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos, 5);
	projectile4 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos, 5);
	projectile5 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos, 5);
	projectileDestination = character->getPos() - pos;
	projectileDestination = VMath::normalize(Vec3(projectileDestination.x, projectileDestination.y, 1));
}

bool FinalEnemy::OnCreate() {
	BuildProjectile();
	//GenerateProjectileDirection();
	return true; 
}
void FinalEnemy::OnDestroy() {}				  /// Just a stub
void FinalEnemy::Update(float deltaTime_) {
	if (attackPattern == 0) {
		Vec3 normalTowardsPlayer = pos - character->getPos();
		normalTowardsPlayer = VMath::normalize(Vec3(normalTowardsPlayer.x, normalTowardsPlayer.y, 1));
		normalTowardsPlayer = normalTowardsPlayer * 10;
		vel = Vec3(-normalTowardsPlayer.x, -normalTowardsPlayer.y, 0);
	}
	if (attackPattern == 1) {
		Vec3 normalTowardsPlayer = character->getPos() - pos;
		normalTowardsPlayer = VMath::normalize(Vec3(normalTowardsPlayer.x, normalTowardsPlayer.y, 1));
		//vel = Vec3(-normalTowardsPlayer.x, -normalTowardsPlayer.y, 0);
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
		if (projectile1->ProjectileDynamicUpdate(projectileDestination, 0.1)) { // so once it hits the wall
			projectile1->setPos(pos); //the pos is reset to the pos of the enemy
			projectile1->setOver(false); //and the moveOver is reset
			projectileDestination = normalTowardsPlayer;
			//projectileDestination = Vec3(character->getPos().x, character->getPos().y, character->getPos().z);
		}
		projectile2->setPos(Vec3(0.0f, 0.0f, -50));
		projectile3->setPos(Vec3(0.0f, 0.0f, -50));
		projectile4->setPos(Vec3(0.0f, 0.0f, -50));
		projectile5->setPos(Vec3(0.0f, 0.0f, -50));
	}
	if (attackPattern == 2) {
		if (projectile2->ProjectileUpdate4Axis(Vec3(projectile2->getPos().x, 100, 0))) { // so once it hits the wall
			projectile2->setPos(pos); //the pos is reset to the pos of the enemy
			projectile2->setOver(false); //and the moveOver is reset
		}
		if (projectile3->ProjectileUpdate4Axis(Vec3(projectile3->getPos().x, -100, 0))) {
			projectile3->setPos(pos);
			projectile3->setOver(false);
		}
		if (projectile4->ProjectileUpdate4Axis(Vec3(100, projectile4->getPos().y, 0))) {
			projectile4->setPos(pos);
			projectile4->setOver(false);
		}
		if (projectile5->ProjectileUpdate4Axis(Vec3(-100, projectile5->getPos().y, 0))) {
			projectile5->setPos(pos);
			projectile5->setOver(false);
		}
		projectile1->setPos(Vec3(0.0f, 0.0f, -50)); //the pos is reset to the pos of the enemy
	}
	if (attackPattern != 1 && attackPattern != 2) {
		projectile1->setPos(Vec3(0.0f,0.0f,-50)); //the pos is reset to the pos of the enemy
		projectile2->setPos(Vec3(0.0f, 0.0f, -50));
		projectile3->setPos(Vec3(0.0f, 0.0f, -50));
		projectile4->setPos(Vec3(0.0f, 0.0f, -50));
		projectile5->setPos(Vec3(0.0f, 0.0f, -50));
	}
	Physics::SimpleNewtonMotion(*this, 0.01);
	projectile1->setModelMatrix(MMath::translate(projectile1->getPos()) * MMath::scale(1.7f, 1.7f, 1.7f)); //setting modelmatrix of the projectile
	projectile2->setModelMatrix(MMath::translate(projectile2->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f)); //setting modelmatrix of the projectile
	projectile3->setModelMatrix(MMath::translate(projectile3->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f));
	projectile4->setModelMatrix(MMath::translate(projectile4->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f));
	projectile5->setModelMatrix(MMath::translate(projectile5->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f));
}

void FinalEnemy::Render() const {
	projectile1->Render();
	projectile2->Render();
	projectile3->Render();
	projectile4->Render();
	projectile5->Render();
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

void FinalEnemy::HandleEvents(const SDL_Event& event) {
	
} /// Just a stub

bool FinalEnemy::DamageCheck(Character* character) { //function to detect dmg
	bool damageTrue = false;
	if (projectile1->DamageCheck(character)) {//check dmg of projectile
		damageTrue = true;
	}
	if (VMath::distance(character->getPos(), pos) < 1) { //if the enemy is overlaping the player
		damageTrue = true;
	}
	return damageTrue;
}