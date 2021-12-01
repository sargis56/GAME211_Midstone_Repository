#include "ProjectileDynamic.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>

ProjectileDynamic::ProjectileDynamic(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_, Vec3 pos_, int damage_, float radius_) :
	mesh(mesh_), shader(shader_), texture(texture_), room(room_) {
	pos = pos_;
	damage = damage_;
	radius = radius_;
}

ProjectileDynamic::~ProjectileDynamic() {}

bool ProjectileDynamic::OnCreate() {
	//damage = 5;
	return true; 
}
void ProjectileDynamic::OnDestroy() {}				  /// Just a stub
void ProjectileDynamic::Update(float deltaTime_) {
	
}

void ProjectileDynamic::Render() const {
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

void ProjectileDynamic::HandleEvents(const SDL_Event& event) {
	
} /// Just a stub

bool ProjectileDynamic::ProjectileDynamicUpdate(Vec3 direction, float deltaTime) { //moves projectile towards the direction vector
	vel.x = direction.x;
	vel.y = direction.y;
	//desiredPos = direction;
	Physics::SimpleNewtonMotion(*this, deltaTime);
			if (room.InsideCollisionPosX(Vec3(pos.x + 0.1f, pos.y, pos.z), 0) == false) { //Collision check
				moveOver = true;
			}
			if (room.InsideCollisionNegX(Vec3(pos.x - 0.1f, pos.y, pos.z), 0) == false) { //Collision check
				moveOver = true;
			}
			if (room.InsideCollisionPosY(Vec3(pos.x, pos.y + 0.1, pos.z), 0) == false) { //Collision check
				moveOver = true;
			}
			if (room.InsideCollisionNegY(Vec3(pos.x, pos.y - 0.1, pos.z), 0) == false) { //Collision check
				moveOver = true;
			}
	return moveOver;
}

bool ProjectileDynamic::DamageCheck(Character* character) { //function to detect dmg
	if (VMath::distance(character->getPos(), pos) <= radius) { //if the projectile is overlapping player
		moveOver = true;
		return true; 
	}
	else {
		return false;
	}
}

