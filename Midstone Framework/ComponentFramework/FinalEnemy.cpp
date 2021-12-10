#include "FinalEnemy.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>
#include "ObjLoader.h"

FinalEnemy::FinalEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_, Character* character_, int timer_) :
	mesh(mesh_), shader(shader_), texture(texture_), room(room_), character(character_) {
	attackTimer = timer_;
}

FinalEnemy::~FinalEnemy() {}

void FinalEnemy::BuildProjectile() {
	ObjLoader::loadOBJ("meshes/Sphere.obj");
	meshProjectile = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderProjectile = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	textureProjectile = new Texture();
	textureProjectile->LoadImage("textures/red.jpg");
	projectile1 = new ProjectileDynamic(meshProjectile, shaderProjectile, textureProjectile, room, pos, 3, 2.0f);
	projectile2 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos, 5);
	projectile3 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos, 5);
	projectile4 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos, 5);
	projectile5 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos, 5);
	projectile6 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos, 5);
	projectile7 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos, 5);
	projectile8 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos, 5);
	projectile9 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos, 5);
	projectileDestination = character->getPos() - pos;
	projectileDestination = VMath::normalize(Vec3(projectileDestination.x, projectileDestination.y, 1));
}

bool FinalEnemy::OnCreate() {
	BuildProjectile();
	attackPattern = 0;
	//attackTimer = 0;
	//GenerateProjectileDirection();
	return true; 
}
void FinalEnemy::OnDestroy() {}				  /// Just a stub
void FinalEnemy::Update(float deltaTime_) {
	Vec3 normalTowardsPlayer = pos - character->getPos();
	normalTowardsPlayer = VMath::normalize(Vec3(normalTowardsPlayer.x, normalTowardsPlayer.y, 1));
	angle = angle = atan2(normalTowardsPlayer.y, normalTowardsPlayer.x) * RADIANS_TO_DEGREES;
	if (textureChange == true) {
		textureChangeTimer++;
		//printf("%i\n", textureChangeTimer);
	}
	attackTimer++;
	if (attackTimer == 300) {
		vel = Vec3(0, 0, 0);
		projectile1->setPos(pos);
		projectile2->setPos(pos);
		projectile3->setPos(pos);
		projectile4->setPos(pos);
		projectile5->setPos(pos);
		projectile6->setPos(pos);
		projectile7->setPos(pos);
		projectile8->setPos(pos);
		projectile9->setPos(pos);
		attackPattern = (rand() % 3);
		attackTimer = 0;
	}
	if (attackPattern == 0) {
		normalTowardsPlayer = normalTowardsPlayer * 10;
		vel = Vec3(-normalTowardsPlayer.x, -normalTowardsPlayer.y, 0);
	}
	if (attackPattern == 1) {
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
		if (projectile1->ProjectileDynamicUpdate(-projectileDestination, 0.1)) { // so once it hits the wall
			projectile1->setPos(pos); //the pos is reset to the pos of the enemy
			projectile1->setOver(false); //and the moveOver is reset
			projectileDestination = normalTowardsPlayer;
			//projectileDestination = Vec3(character->getPos().x, character->getPos().y, character->getPos().z);
		}
		projectile2->setPos(Vec3(0.0f, 0.0f, -50));
		projectile3->setPos(Vec3(0.0f, 0.0f, -50));
		projectile4->setPos(Vec3(0.0f, 0.0f, -50));
		projectile5->setPos(Vec3(0.0f, 0.0f, -50));
		projectile6->setPos(Vec3(0.0f, 0.0f, -50));
		projectile7->setPos(Vec3(0.0f, 0.0f, -50));
		projectile8->setPos(Vec3(0.0f, 0.0f, -50));
		projectile9->setPos(Vec3(0.0f, 0.0f, -50));
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
		if (projectile6->ProjectileUpdate8Axis(Vec3(100, 100, 0))) {
			projectile6->setPos(pos);
			projectile6->setOver(false);
		}
		if (projectile7->ProjectileUpdate8Axis(Vec3(-100, 100, 0))) {
			projectile7->setPos(pos);
			projectile7->setOver(false);
		}
		if (projectile8->ProjectileUpdate8Axis(Vec3(100, -100, 0))) {
			projectile8->setPos(pos);
			projectile8->setOver(false);
		}
		if (projectile9->ProjectileUpdate8Axis(Vec3(-100, -100, 0))) {
			projectile9->setPos(pos);
			projectile9->setOver(false);

		}
		projectile1->setPos(Vec3(0.0f, 0.0f, -50)); //the pos is reset to the pos of the enemy
	}
	if (attackPattern != 1 && attackPattern != 2) {
		projectile1->setPos(Vec3(0.0f,0.0f,-50)); //the pos is reset to the pos of the enemy
		projectile2->setPos(Vec3(0.0f, 0.0f, -50));
		projectile3->setPos(Vec3(0.0f, 0.0f, -50));
		projectile4->setPos(Vec3(0.0f, 0.0f, -50));
		projectile5->setPos(Vec3(0.0f, 0.0f, -50));
		projectile6->setPos(Vec3(0.0f, 0.0f, -50));
		projectile7->setPos(Vec3(0.0f, 0.0f, -50));
		projectile8->setPos(Vec3(0.0f, 0.0f, -50));
		projectile9->setPos(Vec3(0.0f, 0.0f, -50));
	}
	Physics::SimpleNewtonMotion(*this, 0.01);
	projectile1->setModelMatrix(MMath::translate(projectile1->getPos()) * MMath::scale(1.7f, 1.7f, 1.7f)); //setting modelmatrix of the projectile
	projectile2->setModelMatrix(MMath::translate(projectile2->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f)); //setting modelmatrix of the projectile
	projectile3->setModelMatrix(MMath::translate(projectile3->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f));
	projectile4->setModelMatrix(MMath::translate(projectile4->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f));
	projectile5->setModelMatrix(MMath::translate(projectile5->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f));
	projectile6->setModelMatrix(MMath::translate(projectile6->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f));
	projectile7->setModelMatrix(MMath::translate(projectile7->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f));
	projectile8->setModelMatrix(MMath::translate(projectile8->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f));
	projectile9->setModelMatrix(MMath::translate(projectile9->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f));
}

void FinalEnemy::Render() const {
	projectile1->Render();
	projectile2->Render();
	projectile3->Render();
	projectile4->Render();
	projectile5->Render();
	projectile6->Render();
	projectile7->Render();
	projectile8->Render();
	projectile9->Render();
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

bool FinalEnemy::WeaponColCheck(Character* character) {
	if (VMath::distance(character->getPos(), pos) < (character->getCurrentWeaponReach())) { //
		return true;
	}
	else {
		return false;
	}
}
void FinalEnemy::TakeDamage(float damage) {
	health = health - damage;
	//printf("%f", health);
	textureChange = true;
	if (health <= 0) {
		isDead = false;
	}
}

void FinalEnemy::ResetTimer() {
	textureChangeTimer = 0;
	textureChange = false;
}

bool FinalEnemy::DamageCheck(Character* character) { //function to detect dmg
	bool damageTrue = false;
	if (projectile1->DamageCheck(character)) {//check dmg of projectile
		damageTrue = true;
	}
	if (projectile2->DamageCheck(character)) {//check dmg of projectile
		damageTrue = true;
	}
	if (projectile3->DamageCheck(character)) {//check dmg of projectile
		damageTrue = true;
	}
	if (projectile4->DamageCheck(character)) {//check dmg of projectile
		damageTrue = true;
	}
	if (projectile5->DamageCheck(character)) {//check dmg of projectile
		damageTrue = true;
	}
	if (projectile6->DamageCheck(character)) {//check dmg of projectile
		damageTrue = true;
	}
	if (projectile7->DamageCheck(character)) {//check dmg of projectile
		damageTrue = true;
	}
	if (projectile8->DamageCheck(character)) {//check dmg of projectile
		damageTrue = true;
	}
	if (projectile9->DamageCheck(character)) {//check dmg of projectile
		damageTrue = true;
	}
	if (VMath::distance(character->getPos(), pos) < 1) { //if the enemy is overlaping the player
		damageTrue = true;
	}
	return damageTrue;
}