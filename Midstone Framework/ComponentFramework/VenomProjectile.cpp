#include "VenomProjectile.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include "VMath.h"
#include <SDL.h>
#include <cstdlib>

VenomProjectile::VenomProjectile(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_) :
	mesh(mesh_), shader(shader_), texture(texture_), room(room_) {
	//pos = pos_;
}

VenomProjectile::~VenomProjectile() {}

bool VenomProjectile::OnCreate() {


	return true;
}
void VenomProjectile::OnDestroy() {}				  /// Just a stub
void VenomProjectile::Update(float deltaTime_) {
	 
	
}

void VenomProjectile::Render() const {
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

void VenomProjectile::HandleEvents(const SDL_Event& event) {

} /// Just a stub


void VenomProjectile::VProjectileUpdate(Vec3 target) {

	float speed = 0.04f;
	Vec3 normalDir = target;
	normalDir.Normalize();
	pos += (normalDir * speed);



//	pos = VMath::lerp(pos, target, (0.5 * speed));
	
	
	//desiredPos = direction;
	//if (pos != target) { //if the desired pos is not reached continue to move
	//	if (pos.x && pos.y >= target.x) {
	//		if (room.InsideCollisionPosX(Vec3(pos.x + 0.1f, pos.y, pos.z), 0)) { //Collision check
	//			pos.x = pos.x + 0.05f;
	//		}
	//		else { //If moving in a wall, gen new move
	//			moveOver = true;
	//		}
	//	}
	//	else if (pos.x > target.x) {
	//		if (room.InsideCollisionNegX(Vec3(pos.x - 0.1f, pos.y, pos.z), 0)) {
	//			pos.x = pos.x - 0.05f;
	//		}
	//		else {
	//			moveOver = true;
	//		}
	//	}
	//	else if (pos.y < target.y) {
	//		if (room.InsideCollisionPosY(Vec3(pos.x, pos.y + 0.1, pos.z), 0)) {
	//			pos.y = pos.y + 0.05f;
	//		}
	//		else {
	//			moveOver = true;
	//		}
	//	}
	//	else if (pos.y > target.y) {
	//		if (room.InsideCollisionNegY(Vec3(pos.x, pos.y - 0.1, pos.z), 0)) {
	//			pos.y = pos.y - 0.05f;
	//		}
	//		else {
	//			moveOver = true;
	//		}
	//	}
	//}
	
}

bool VenomProjectile::DamageCheck(Character* character) {
	if (VMath::distance(character->getPos(), pos) < 0.5) {
		return true;
	}
	else {
		return false;
	}
}
