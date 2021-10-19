#include "MageTurretEnemy.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>
#include "ObjLoader.h"

MageTurretEnemy::MageTurretEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_) :
	mesh(mesh_), shader(shader_), texture(texture_), room(room_) {
}

MageTurretEnemy::~MageTurretEnemy() {}

void MageTurretEnemy::BuildProjectile() {
	ObjLoader::loadOBJ("meshes/Sphere.obj");
	meshProjectile = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderProjectile = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	textureProjectile = new Texture();
	textureProjectile->LoadImage("textures/skull_texture.jpg");
	projectile1 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos);//setting up the projectile classes
	projectile2 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos);
	projectile3 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos);
	projectile4 = new Projectile(meshProjectile, shaderProjectile, textureProjectile, room, pos);


}

bool MageTurretEnemy::OnCreate() {
	BuildProjectile();
	return true; 
}
void MageTurretEnemy::OnDestroy() {}				  /// Just a stub
void MageTurretEnemy::Update(float deltaTime_) {
	if (projectile1->ProjectileUpdate4Axis(Vec3(projectile1->getPos().x,100,0))){ // so once it hits the wall
		projectile1->setPos(pos); //the pos is reset to the pos of the enemy
		projectile1->setOver(false); //and the moveOver is reset
	}
	if (projectile2->ProjectileUpdate4Axis(Vec3(projectile1->getPos().x, -100, 0))) {
		projectile2->setPos(pos);
		projectile2->setOver(false);
	}
	if (projectile3->ProjectileUpdate4Axis(Vec3(100, projectile1->getPos().y, 0))) {
		projectile3->setPos(pos);
		projectile3->setOver(false);
	}
	if (projectile4->ProjectileUpdate4Axis(Vec3(-100, projectile1->getPos().y, 0))) {
		projectile4->setPos(pos);
		projectile4->setOver(false);
	}
	
	projectile1->setModelMatrix(MMath::translate(projectile1->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f)); //setting modelmatrix of the projectile
	projectile2->setModelMatrix(MMath::translate(projectile2->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f));
	projectile3->setModelMatrix(MMath::translate(projectile3->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f));
	projectile4->setModelMatrix(MMath::translate(projectile4->getPos()) * MMath::scale(0.4f, 0.4f, 0.4f));
}

void MageTurretEnemy::Render() const {
	projectile1->Render();
	projectile2->Render();
	projectile3->Render();
	projectile4->Render();
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

void MageTurretEnemy::HandleEvents(const SDL_Event& event) {
	
} /// Just a stub

bool MageTurretEnemy::DamageCheck(Character* character) { //function to detect dmg
	projectile1->DamageCheck(character);//check dmg of projectile
	projectile2->DamageCheck(character);
	projectile3->DamageCheck(character);
	projectile4->DamageCheck(character);
	if (VMath::distance(character->getPos(), pos) < 1) { //if the enemy is overlaping the player
		return true;
	}
	else {
		return false;
	}
}

