#include "Projectile.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>

Projectile::Projectile(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_, Vec3 pos_, int damage_) :
	mesh(mesh_), shader(shader_), texture(texture_), room(room_) {
	pos = pos_;
	damage = damage_; //constructor
}

Projectile::~Projectile() {}

bool Projectile::OnCreate() {
	//damage = 5; //test int for damage value
	return true; 
}
void Projectile::OnDestroy() {}				  //inherited function
void Projectile::Update(float deltaTime_) {} //inherited function

void Projectile::Render() const {
	Matrix3 normalMatrix = MMath::transpose(MMath::inverse(modelMatrix)); //set normalmatrix
	glUniformMatrix4fv(shader->getUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix); //GLSL uniform pass
	glUniformMatrix3fv(shader->getUniformID("normalMatrix"), 1, GL_FALSE, normalMatrix); //GLSL uniform pass
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	}
	mesh->Render(); //call the mesh render

	/// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0); //discard texture
}

void Projectile::HandleEvents(const SDL_Event& event) {} //inherited function

bool Projectile::ProjectileUpdate4Axis(Vec3 direction) { //moves projectile towards the direction vector
	//desiredPos = direction;
	if (pos != direction) { //if the desired pos is not reached continue to move
		if (pos.x < direction.x) {
			if (room.InsideCollisionPosX(Vec3(pos.x + 0.1f, pos.y, pos.z), 0)) { //Collision check
				pos.x = pos.x + 0.05f;
			}
			else { //If moving in a wall, reset the Projectile
				moveOver = true;
			}
		}
		else if (pos.x > direction.x) {
			if (room.InsideCollisionNegX(Vec3(pos.x - 0.1f, pos.y, pos.z), 0)) { //Collision check
				pos.x = pos.x - 0.05f;
			}
			else { //If moving in a wall, reset the Projectile
				moveOver = true;
			}
		}
		else if (pos.y < direction.y) {
			if (room.InsideCollisionPosY(Vec3(pos.x, pos.y + 0.1, pos.z), 0)) { //Collision check
				pos.y = pos.y + 0.05f;
			}
			else { //If moving in a wall, reset the Projectile
				moveOver = true;
			}
		}
		else if (pos.y > direction.y) {
			if (room.InsideCollisionNegY(Vec3(pos.x, pos.y - 0.1, pos.z), 0)) { //Collision check
				pos.y = pos.y - 0.05f;
			}
			else { //If moving in a wall, reset the Projectile
				moveOver = true;
			}
		}
	}
	return moveOver;
}

bool Projectile::ProjectileUpdate8Axis(Vec3 direction) { // same function as above but with 8 axises of movement
	//desiredPos = direction;
	if (pos != direction) { //if the desired pos is not reached continue to move
		if (pos.x < direction.x) {
			if (room.InsideCollisionPosX(Vec3(pos.x + 0.1f, pos.y, pos.z), 0)) { //Collision check
				pos.x = pos.x + 0.05f;
			}
			else { //If moving in a wall, reset the Projectile
				moveOver = true;
			}
		}
		if (pos.x > direction.x) {
			if (room.InsideCollisionNegX(Vec3(pos.x - 0.1f, pos.y, pos.z), 0)) { //Collision check
				pos.x = pos.x - 0.05f;
			}
			else { //If moving in a wall, reset the Projectile
				moveOver = true;
			}
		}
		if (pos.y < direction.y) {
			if (room.InsideCollisionPosY(Vec3(pos.x, pos.y + 0.1, pos.z), 0)) { //Collision check
				pos.y = pos.y + 0.05f;
			}
			else { //If moving in a wall, reset the Projectile
				moveOver = true;
			}
		}
		if (pos.y > direction.y) {
			if (room.InsideCollisionNegY(Vec3(pos.x, pos.y - 0.1, pos.z), 0)) { //Collision check
				pos.y = pos.y - 0.05f;
			}
			else { //If moving in a wall, reset the Projectile
				moveOver = true;
			}
		}
	}
	return moveOver;
}

bool Projectile::DamageCheck(Character* character) { //function to detect dmg
	if (VMath::distance(character->getPos(), pos) < 0.5) { //if the projectile is overlapping player
		moveOver = true;
		return true; 
	}
	else {
		return false;
	}
}

