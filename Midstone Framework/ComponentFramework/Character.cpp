#include "Character.h"
#include "ObjLoader.h"

#include "MMath.h"
#include <SDL.h>

Character::Character(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_) :
	room(room_), mesh(mesh_), shader(shader_), texture(texture_) {
}

Character::~Character() {}

bool Character::OnCreate() { 
	//health = 50;
	return true;
}
void Character::OnDestroy() {}				  /// Just a stub
void Character::Update(float deltaTime_) {} /// Just a stub

void Character::Render() const {
	Matrix3 normalMatrix = MMath::transpose(MMath::inverse(modelMatrix));
	/// This is some fancy code.  Assigning a 4x4 matrix to a 3x3 matrix
	/// just steals the upper 3x3 of the 4x4 and assigns thoses values 
	/// to the 3x3.  
	//Matrix3 normalMatrix = MMath::transpose(MMath::inverse(modelMatrix));
	glUniformMatrix4fv(shader->getUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
	glUniformMatrix3fv(shader->getUniformID("normalMatrix"), 1, GL_FALSE, normalMatrix);
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	}
	mesh->Render();

	/// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Character::HandleEvents(const SDL_Event& event) {
	//	switch (event.key.keysym.scancode) {
	//	case SDL_SCANCODE_W:
	//		if (room.InsideCollisionPosY(Vec3(pos.x, pos.y + 0.1f, pos.z), 0)) { //collision check
	//			pos.y = pos.y + 0.1f; //adding position
	//		}
	//		break;
	//	case SDL_SCANCODE_D:
	//		if (room.InsideCollisionPosX(Vec3(pos.x + 0.1f, pos.y, pos.z), 0)) {
	//			pos.x = pos.x + 0.1f;
	//		}
	//		break;
	//	case SDL_SCANCODE_A:
	//		if (room.InsideCollisionNegX(Vec3(pos.x - 0.1f, pos.y, pos.z), 0)) {
	//			pos.x = pos.x - 0.1f;
	//		}
	//		break;
	//	case SDL_SCANCODE_S:
	//		if (room.InsideCollisionNegY(Vec3(pos.x, pos.y - 0.1f, pos.z), 0)) {
	//			pos.y = pos.y - 0.1f;
	//		}
	//		break;
	//	default:
	//		break;
	//}

		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_W:
				keyUp = true;
				break;
			case SDL_SCANCODE_D:
				keyR = true;
				break;
			case SDL_SCANCODE_A:
				keyL = true;
				break;
			case SDL_SCANCODE_S:
				keyDown = true;
				break;
			default:
				break;
			}
		}

		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_W:
				keyUp = false;
				break;
			case SDL_SCANCODE_D:
				keyR = false;
				break;

			case SDL_SCANCODE_A:
				keyL = false;
				break;
			case SDL_SCANCODE_S:
				keyDown = false;
				break;
			default:
				break;
			}
		}

		if (keyUp == true)
		{
			if (room.InsideCollisionPosY(Vec3(pos.x, pos.y + speed, pos.z), 0)) { //collision check
				pos.y = pos.y + speed; //adding position
			}
		}
		

		if (keyR == true)
		{
			if (room.InsideCollisionPosX(Vec3(pos.x + speed, pos.y, pos.z), 0)) {
				pos.x = pos.x + speed;
			}
		}

		if (keyL == true)
		{
			if (room.InsideCollisionNegX(Vec3(pos.x - speed, pos.y, pos.z), 0)) {
				pos.x = pos.x - speed;
			}
		}

		if (keyDown == true)
		{
			if (room.InsideCollisionNegY(Vec3(pos.x, pos.y - speed, pos.z), 0)) {
				pos.y = pos.y - speed;
			}
		}
}

void Character::checkInvincibility() {
	if (invincibilityTimer >= 0) {
		invincibilityTimer--; //timer varaible function
		//visiablity shit
		if ((invincibilityTimer & 1) == 0) {
			visibility = false;
		}
		else {
			visibility = true;
		}
		invincibility = true;
	}
	else {
		invincibility = false;
	}
	//if ((timer & 1) == 0) {
	//	printf("wTF\n");
	//}
}
