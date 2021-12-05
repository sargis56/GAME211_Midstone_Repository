#include "Weapon.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>

Weapon::Weapon(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_) :
	mesh(mesh_), shader(shader_), texture(texture_) {
	pos = pos_;
	weaponDropActive = true;
}

Weapon::Weapon(){

}

Weapon::~Weapon() {}

bool Weapon::OnCreate() {


	return true;
}
void Weapon::OnDestroy() {}				  /// Just a stub
void Weapon::Update(float deltaTime_) {
}

void Weapon::Render() const {
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

void Weapon::HandleEvents(const SDL_Event& event) {

} /// Just a stub

bool Weapon::collisionCheck(Character* character) {
	if (VMath::distance(character->getPos(), pos) < 1) {
		printf("\nWeapon with ID: %i has been picked up with stats:\n", weaponID);
		printf("Swing Damage: %f | Stab Damage: %f | Reach %f | Delay: %f\n", swingDamage, stabDamage, reach, delay);

		character->setCurrentWeaponID(weaponID);
		character->setAltWeaponMode(altMode);

		character->setCurrentDamageType(damageType);
		character->setCurrentSwingDamage(swingDamage);
		character->setCurrentStabDamage(stabDamage);
		character->setCurrentWeaponDelay(delay);
		character->setCurrentWeaponReach(reach);

		character->setWeaponDamageType(damageType);
		character->setSwingDamage(swingDamage);
		character->setStabDamage(stabDamage);
		character->setWeaponDelay(delay);
		character->setWeaponReach(reach);

		if (altMode == true)
		{
			character->setAltWeaponDamageType(altDamageType);
			character->setAltSwingDamage(altSwingDamage);
			character->setAltStabDamage(altStabDamage);
			character->setAltWeaponDelay(altDelay);
			character->setAltWeaponReach(altReach);
		}
		else
		{
			character->setAltWeaponDamageType(0);
			character->setAltSwingDamage(0);
			character->setAltStabDamage(0);
			character->setAltWeaponDelay(0);
			character->setAltWeaponReach(0);
		}

		printf("\nPlayer wepaon stats have been changed: Swing Damage: %f | Stab Damage: %f | Reach %f | Delay: %f\n", character->getCurrentSwingDamage(), character->getCurrentStabDamage(), character->getCurrentWeaponReach(), character->getCurrentWeaponDelay());

		weaponDropActive = false;
		return true;
	}
	return false;
}

void Weapon::setWeapon(Character* character) {
	printf("\nWeapon with ID: %i has been picked up with stats:\n", weaponID);
	printf("Swing Damage: %f | Stab Damage: %f | Reach %f | Delay: %f\n", swingDamage, stabDamage, reach, delay);

	character->setCurrentWeaponID(weaponID);
	character->setAltWeaponMode(altMode);

	character->setCurrentDamageType(damageType);
	character->setCurrentSwingDamage(swingDamage);
	character->setCurrentStabDamage(stabDamage);
	character->setCurrentWeaponDelay(delay);
	character->setCurrentWeaponReach(reach);

	character->setWeaponDamageType(damageType);
	character->setSwingDamage(swingDamage);
	character->setStabDamage(stabDamage);
	character->setWeaponDelay(delay);
	character->setWeaponReach(reach);

	if (altMode == true)
	{
		character->setAltWeaponDamageType(altDamageType);
		character->setAltSwingDamage(altSwingDamage);
		character->setAltStabDamage(altStabDamage);
		character->setAltWeaponDelay(altDelay);
		character->setAltWeaponReach(altReach);
	}
	else
	{
		character->setAltWeaponDamageType(0);
		character->setAltSwingDamage(0);
		character->setAltStabDamage(0);
		character->setAltWeaponDelay(0);
		character->setAltWeaponReach(0);
	}

	printf("\nPlayer wepaon stats have been changed: Swing Damage: %f | Stab Damage: %f | Reach %f | Delay: %f\n", character->getCurrentSwingDamage(), character->getCurrentStabDamage(), character->getCurrentWeaponReach(), character->getCurrentWeaponDelay());

	weaponDropActive = false;
}