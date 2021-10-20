#include "TeslaTowerEnemy.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>
#include "ObjLoader.h"

TeslaTowerEnemy::TeslaTowerEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_, Character* character_) :
	mesh(mesh_), shader(shader_), texture(texture_), room(room_), character(character_) {
}

TeslaTowerEnemy::~TeslaTowerEnemy() {}

void TeslaTowerEnemy::BuildProjectile() {
	ObjLoader::loadOBJ("meshes/Sphere.obj");
	meshProjectile = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderProjectile = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	textureProjectile = new Texture();
	textureProjectile->LoadImage("textures/skull_texture.jpg");
	projectile = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos, 10);
}

bool TeslaTowerEnemy::OnCreate() {
	BuildProjectile();
	GenerateProjectileDirection();
	return true; 
}
void TeslaTowerEnemy::OnDestroy() {}				  /// Just a stub
void TeslaTowerEnemy::Update(float deltaTime_) {
	if (projectile->ProjectileUpdate8Axis(Vec3(projectileDestination))){ // so once it hits the wall
		projectile->setPos(pos); //the pos is reset to the pos of the enemy
		projectile->setOver(false); //and the moveOver is reset
		GenerateProjectileDirection();
		//projectileDestination = Vec3(character->getPos().x, character->getPos().y, character->getPos().z);
	}
	
	projectile->setModelMatrix(MMath::translate(projectile->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f)); //setting modelmatrix of the projectile
}

void TeslaTowerEnemy::Render() const {
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

void TeslaTowerEnemy::HandleEvents(const SDL_Event& event) {
	
} /// Just a stub

Vec3 TeslaTowerEnemy::GenerateProjectileDirection() { //generate a new direction for the projectile
	if (character->getPos().x <= 0) { //if the pos is in the negative quadrant
		projectileDestination.x = -100.0f; //set the destination on x to the negative edge
	}
	else {
		projectileDestination.x = 100.0f; //else set the destination on x to the postive edge
	}
	if (character->getPos().y <= 0) {
		projectileDestination.y = -100.0f;//set the destination on y to the negative edge
	}
	else {
		projectileDestination.y = 100.0f;//else set the destination on y to the postive edge
	}
	projectileDestination.z = character->getPos().z; //default the z component
	return projectileDestination;
}

bool TeslaTowerEnemy::DamageCheck(Character* character) { //function to detect dmg
	projectile->DamageCheck(character);//check dmg of projectile
	if (VMath::distance(character->getPos(), pos) < 1) { //if the enemy is overlaping the player
		return true;
	}
	else {
		return false;
	}
}