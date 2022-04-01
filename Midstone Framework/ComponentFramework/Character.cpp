#include "Character.h"
#include "ObjLoader.h"
#include "Weapon.h"
#include "MMath.h"
#include <SDL.h>

#include "Shovel.h"
#include "Axe.h"
#include "Sword.h"
#include "Zweihander.h"
#include "Maul.h"
#include "Hammer.h"
#include "WarAxe.h"
#include "Halbred.h"
#include "Spear.h"

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


	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if(event.button.button == SDL_BUTTON_LEFT)
		{
			swing = true;
			attacking = true;
			damageFromPlayer = getCurrentSwingDamage();
			//printf("\n-SWING-\n");
		}
		if (event.button.button == SDL_BUTTON_RIGHT)
		{
			stab = true;
			attacking = true;
			damageFromPlayer = getCurrentStabDamage();
			//printf("\n-STAB-\n");
		}
		if (event.button.button == SDL_BUTTON_MIDDLE)
		{
			//printf("\n-ALT MODE-");
			if (altWeaponDamageType != 0)
			{
				if (altWeaponMode == false)
				{
					altWeaponMode = true;
					//printf("-ON-\n");
				}
				else
				{
					altWeaponMode = false;
					//printf("-OFF-\n");
				}
			}
			else
			{
				altWeaponMode = false;
				//printf("-OFF-\n");
			}
			
		}
	}

	if (event.type == SDL_MOUSEBUTTONUP)
	{
		//printf("\n-N0 ATTACKS-\n");
		stab = false;
		swing = false;
		attacking = false;
	}


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

			case SDL_SCANCODE_UP:
				keyUp = true;
				break;
			case SDL_SCANCODE_RIGHT:
				keyR = true;
				break;
			case SDL_SCANCODE_LEFT:
				keyL = true;
				break;
			case SDL_SCANCODE_DOWN:
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

			case SDL_SCANCODE_UP:
				keyUp = false;
				break;
			case SDL_SCANCODE_RIGHT:
				keyR = false;
				break;

			case SDL_SCANCODE_LEFT:
				keyL = false;
				break;
			case SDL_SCANCODE_DOWN:
				keyDown = false;
				break;

			default:
				break;
			}
		}

		if (keyUp == true)
		{
			rotation = 180;
			if (room.InsideCollisionPosY(Vec3(pos.x, pos.y + speed, pos.z), 0)) { //collision check
				pos.y = pos.y + speed; //adding position
			}
		}
		

		if (keyR == true)
		{
			rotation = 90;
			if (room.InsideCollisionPosX(Vec3(pos.x + speed, pos.y, pos.z), 0)) {
				pos.x = pos.x + speed;
			}
		}

		if (keyL == true)
		{
			rotation = -90;
			if (room.InsideCollisionNegX(Vec3(pos.x - speed, pos.y, pos.z), 0)) {
				pos.x = pos.x - speed;
			}
		}

		if (keyDown == true)
		{
			rotation = 360;
			if (room.InsideCollisionNegY(Vec3(pos.x, pos.y - speed, pos.z), 0)) {
				pos.y = pos.y - speed;
			}
		}

		if ((keyUp == true) && (keyL == true))
		{
			rotation = -135;
		}
		if ((keyUp == true) && (keyR == true))
		{
			rotation = 135;
		}
		if ((keyDown == true) && (keyL == true))
		{
			rotation = -45;
		}
		if ((keyDown == true) && (keyR == true))
		{
			rotation = 45;
		}

		if (altWeaponMode == true)
		{
			currentDamageType = altWeaponDamageType;
			currentSwingDamage = altSwingDamage;
			currentStabDamage = altStabDamage;
			currentWeaponReach = altWeaponReach;
			currentWeaponDelay = altWeaponDelay;
		}
		else
		{
			currentDamageType = weaponDamageType;
			currentSwingDamage = swingDamage;
			currentStabDamage = stabDamage;
			currentWeaponReach = weaponReach;
			currentWeaponDelay = weaponDelay;
		}
		//printf("\nWeapon New ID: %i\n", currentWeaponID);
		//printf("\nWeapon New Swing Damage: %f\n", currentSwingDamage);
}

void Character::checkInvincibility() {
	if (invincibilityTimer >= 0) { //checking if the timer is up
		invincibilityTimer--; //timer varaible function
		if ((invincibilityTimer & 1) == 0) { //makes the visibility flicker
			visibility = false;
		}
		else {
			visibility = true;
		}
		invincibility = true; //invinciblity variable
	}
	else {
		invincibility = false;
	}
}

void Character::setWeapon(int weapon_) {
	Shovel* shovel_ = new Shovel(0);
	Sword* sword_ = new Sword(1);
	Axe* axe_ = new Axe(2);
	Zweihander* zweihander_ = new Zweihander(3);
	Maul* maul_ = new Maul(4);
	Hammer* hammer_ = new Hammer(5);
	WarAxe* warAxe_ = new WarAxe(6);
	Halbred* halbred_ = new Halbred(7);
	Spear* spear_ = new Spear(8);
	Weapon *weapons[]{ shovel_, sword_, axe_, zweihander_, maul_, hammer_, warAxe_, halbred_, spear_ };

	weapons[weapon_]->setWeapon(this);
}


