#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene01.h"
#include "DemoObject.h"
#include "Character.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Debug.h"
#include "Physics.h"
#include "SceneManager.h"
using namespace std;

Scene01::Scene01() : character(nullptr), characterMesh(nullptr), shaderPtr(nullptr), texturePtr(nullptr), boxMesh(nullptr), doorLeft(nullptr), doorTop(nullptr), doorTexture(nullptr), floor(nullptr), floorTexture(nullptr),
health(NULL), healthBar(nullptr), healthUITexture(nullptr), wall1(nullptr), wall2(nullptr), wall3(nullptr), wall4(nullptr), wallTexture(nullptr), speed(NULL), weapon(NULL) {
	//Debug::Info("Created SceneDev: ", __FILE__, __LINE__);
}

Scene01::~Scene01() {}

bool Scene01::OnCreate() {
	//Room setup
	room.setRoomBorders(Vec3(-9.0, -4.0, 0.0), Vec3(9.0, 4.0, 0.0));
	light1 = Vec3(0.0f, 40.0f, 0.0f);
	roomUpdate = roomCleared; //setting local clear
	projectionMatrix = MMath::perspective(30.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	//character onCreate
	BuildCharacter();
	//room onCreate
	BuildRoom();
	BuildHealthUI();
	//enemy onCreate
	BuildAllEnemies();

	//setting modelMatrix for static objs
	healthBar->setModelMatrix(MMath::translate(Vec3(0.0f, -3.5f, -5.0f)) * MMath::scale(0.05f * (health + 0.01), 0.3f, 0.01f) * MMath::rotate(-10.0f, 1.0, 0.0, 0.0));
	wall1->setModelMatrix(MMath::translate(Vec3(-11.0, 0.0, -15.0)) * MMath::scale(0.75f, 5.0f, 1.0f));
	wall2->setModelMatrix(MMath::translate(Vec3(11.0, 0.0, -15.0)) * MMath::scale(0.75f, 5.0f, 1.0f));
	wall3->setModelMatrix(MMath::translate(Vec3(0.0, -5.75, -15.0)) * MMath::scale(11.5f, 0.75f, 1.0f));
	wall4->setModelMatrix(MMath::translate(Vec3(0.0, 5.75, -15.0)) * MMath::scale(11.5f, 0.75f, 1.0f));
	floor->setModelMatrix(MMath::translate(Vec3(0.0, 0.0, -17.0)) * MMath::scale(11.4f, 5.5f, 1.0f));
	doorLeft->setModelMatrix(MMath::translate(doorLeft->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f) * MMath::rotate(90, Vec3(0, 0, 1)));
	doorTop->setModelMatrix(MMath::translate(doorTop->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f));
	hammer->setModelMatrix(MMath::translate(hammer->getPos()) * MMath::scale(0.25f, 0.25f, 0.25f));
	healthpot->setModelMatrix(MMath::translate(healthpot->getPos()) * MMath::scale(0.7f, 0.7f, 0.7f));
	return true;
}

void Scene01::HandleEvents(const SDL_Event& sdlEvent) {
	character->HandleEvents(sdlEvent);
}

int Scene01::SetScene() {
	return sceneNumber;
}

void Scene01::BuildCharacter() {
	ObjLoader::loadOBJ("meshes/Characters/Pl_LordWill.obj");
	characterMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	texturePtr = new Texture();
	texturePtr->LoadImage("textures/Characters/Pl_LordW.png");
	character = new Character(characterMesh, shaderPtr, texturePtr, room);
	character->setSpeed(speed);
	character->setPos(returnedPos); //using for setting the position 
	character->setWeapon(weapon);
}

void Scene01::BuildAllEnemies() {
	ObjLoader::loadOBJ("meshes/Enemies/MagicTurret.obj");
	turretMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	turretTexture = new Texture();
	turretTexture->LoadImage("textures/Enemies/Turret_Texture.jpg");
	turret1 = new MageTurretEnemy(turretMesh, shaderPtr, turretTexture, room);
	turret1->OnCreate();
	turret1->setPos(Vec3(0, 0, -15));
	turret2 = new TeslaTowerEnemy(turretMesh, shaderPtr, turretTexture, room, character);
	turret2->OnCreate();
	turret2->setPos(Vec3(3, 3, -15));
	turret3 = new TeslaTowerEnemy(turretMesh, shaderPtr, turretTexture, room, character);
	turret3->OnCreate();
	turret3->setPos(Vec3(-3, 3, -15));
	turret4 = new TeslaTowerEnemy(turretMesh, shaderPtr, turretTexture, room, character);
	turret4->OnCreate();
	turret4->setPos(Vec3(3, -3, -15));
	turret5 = new TeslaTowerEnemy(turretMesh, shaderPtr, turretTexture, room, character);
	turret5->OnCreate();
	turret5->setPos(Vec3(-3, -3, -15));

	ObjLoader::loadOBJ("meshes/Items/Potion.obj");
	healthPotMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	healthPotTexture = new Texture();
	healthPotTexture->LoadImage("textures/green.jpg");
	healthpot = new HealingItem(healthPotMesh, shaderPtr, healthPotTexture, Vec3(-5.0, 3.0, -15.0));

	ObjLoader::loadOBJ("meshes/Weapons/Hammer.obj");
	hammerMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	weaponTexture = new Texture();
	weaponTexture->LoadImage("textures/green.jpg");
	hammer = new Hammer(hammerMesh, shaderPtr, weaponTexture, Vec3(-5.0f, 0.0f, -15.0f));
}

void Scene01::BuildRoom() {
	ObjLoader::loadOBJ("meshes/Cube.obj");
	boxMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	ObjLoader::loadOBJ("meshes/Scenery/DoorModel.obj");
	doorMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	wallTexture = new Texture();
	wallTexture->LoadImage("textures/cobblestone.jpg");
	floorTexture = new Texture();
	floorTexture->LoadImage("textures/floor.jpg");
	doorTexture = new Texture();
	doorTexture->LoadImage("textures/Scenery/DoorModel_D.png");

	wall1 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall2 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall3 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall4 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	floor = new StaticMesh(boxMesh, shaderPtr, floorTexture);
	doorLeft = new Door(doorMesh, shaderPtr, doorTexture, Vec3(-9.5, 0.0, -15.0f));
	doorTop = new Door(doorMesh, shaderPtr, doorTexture, Vec3(0.0, 4.5, -15.0f));
}
void Scene01::BuildHealthUI() {
	healthUITexture = new Texture();
	healthUITexture->LoadImage("textures/red.jpg");
	healthBar = new HealthUI(boxMesh, shaderPtr, healthUITexture);
}

void Scene01::Update(const float deltaTime) {
	//enemy and item updates
	if (roomUpdate == false) {
		if (hammer->getActive()) {
			hammer->collisionCheck(character);
		}
		if (turret1->isAlive()) {
			turret1->Update(deltaTime);
			if (turret1->getTimer() >= 20) {
				turretTexture->LoadImage("textures/Enemies/Turret_Texture.jpg");
				turret1->ResetTimer();
			}
			if (turret1->DamageCheck(character) && character->getInvincibility() == false && character->getAttacking() == false) {
				character->setinvincibilityTimer(100); //setting the timer for the invinciblity
				health -= 20; //set characters new health after taking damage
			}
			if (turret1->WeaponColCheck(character) && character->getAttacking() == true) {
				//Enemy takes damage
				//ratEnemy->TakeDamage(character->getDamageFromPlayer());
				turret1->TakeDamage(character->getDamageFromPlayer());
				turretTexture->LoadImage("textures/red.jpg");
				//printf("\nEnemy has taken damage");
			}
		}
		if (turret2->isAlive()) {
			turret2->Update(deltaTime);
			if (turret2->getTimer() >= 20) {
				turretTexture->LoadImage("textures/Enemies/Turret_Texture.jpg");
				turret2->ResetTimer();
			}
			if (turret2->DamageCheck(character) && character->getInvincibility() == false && character->getAttacking() == false) {
				character->setinvincibilityTimer(100); //setting the timer for the invinciblity
				health -= 20; //set characters new health after taking damage
			}
			if (turret2->WeaponColCheck(character) && character->getAttacking() == true) {
				//Enemy takes damage
				//ratEnemy->TakeDamage(character->getDamageFromPlayer());
				turret2->TakeDamage(character->getDamageFromPlayer());
				turretTexture->LoadImage("textures/red.jpg");
				//printf("\nEnemy has taken damage");
			}
		}
		if (turret3->isAlive()) {
			turret3->Update(deltaTime);
			if (turret3->getTimer() >= 20) {
				turretTexture->LoadImage("textures/Enemies/Turret_Texture.jpg");
				turret3->ResetTimer();
			}
			if (turret3->DamageCheck(character) && character->getInvincibility() == false && character->getAttacking() == false) {
				character->setinvincibilityTimer(100); //setting the timer for the invinciblity
				health -= 20; //set characters new health after taking damage
			}
			if (turret3->WeaponColCheck(character) && character->getAttacking() == true) {
				//Enemy takes damage
				//ratEnemy->TakeDamage(character->getDamageFromPlayer());
				turret3->TakeDamage(character->getDamageFromPlayer());
				turretTexture->LoadImage("textures/red.jpg");
				//printf("\nEnemy has taken damage");
			}
		}
		if (turret4->isAlive()) {
			turret4->Update(deltaTime);
			if (turret4->getTimer() >= 20) {
				turretTexture->LoadImage("textures/Enemies/Turret_Texture.jpg");
				turret4->ResetTimer();
			}
			if (turret4->DamageCheck(character) && character->getInvincibility() == false && character->getAttacking() == false) {
				character->setinvincibilityTimer(100); //setting the timer for the invinciblity
				health -= 20; //set characters new health after taking damage
			}
			if (turret4->WeaponColCheck(character) && character->getAttacking() == true) {
				//Enemy takes damage
				//ratEnemy->TakeDamage(character->getDamageFromPlayer());
				turret4->TakeDamage(character->getDamageFromPlayer());
				turretTexture->LoadImage("textures/red.jpg");
				//printf("\nEnemy has taken damage");
			}
		}
		if (turret5->isAlive()) {
			turret5->Update(deltaTime);
			if (turret5->getTimer() >= 20) {
				turretTexture->LoadImage("textures/Enemies/Turret_Texture.jpg");
				turret5->ResetTimer();
			}
			if (turret5->DamageCheck(character) && character->getInvincibility() == false && character->getAttacking() == false) {
				character->setinvincibilityTimer(100); //setting the timer for the invinciblity
				health -= 20; //set characters new health after taking damage
			}
			if (turret5->WeaponColCheck(character) && character->getAttacking() == true) {
				//Enemy takes damage
				//ratEnemy->TakeDamage(character->getDamageFromPlayer());
				turret5->TakeDamage(character->getDamageFromPlayer());
				turretTexture->LoadImage("textures/red.jpg");
				//printf("\nEnemy has taken damage");
			}
		}
		if (healthpot->getActive() && healthpot->collisionCheck(character)) {
			health = health + 20;
			if (health > 50) {
				health = 50;
			}
		}
	}
	//door and character updates
	if (turret1->isAlive() == false && turret2->isAlive() == false && turret3->isAlive() == false && turret4->isAlive() == false && turret5->isAlive() == false || roomCleared == true) { //enemies are dead - unlock room
		roomCleared = true;
		if (doorTop->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
			sceneNumber = 7;
		}
		if (doorLeft->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
			sceneNumber = 1;
		}
	}
	if (health <= 0) { //check if the player is dead
		sceneNumber = 31;
	}
	turret1->setModelMatrix(MMath::translate(turret1->getPos())* MMath::scale(Vec3(0.5, 0.5, 0.5f)));
	turret2->setModelMatrix(MMath::translate(turret2->getPos())* MMath::scale(Vec3(0.5, 0.5, 0.5f)));
	turret3->setModelMatrix(MMath::translate(turret3->getPos())* MMath::scale(Vec3(0.5, 0.5, 0.5f)));
	turret4->setModelMatrix(MMath::translate(turret4->getPos())* MMath::scale(Vec3(0.5, 0.5, 0.5f)));
	turret5->setModelMatrix(MMath::translate(turret5->getPos())* MMath::scale(Vec3(0.5, 0.5, 0.5f)));
	character->checkInvincibility(); //checking if the character is invincible
	character->setModelMatrix(MMath::translate(character->getPos()) * MMath::rotate(character->getRotation(), Vec3(0.0f, 0.0f, 1.0f)));
	healthBar->setModelMatrix(MMath::translate(Vec3(0.0f, -3.5f, -5.0f)) * MMath::scale(0.05f * (health + 0.01), 0.3f, 0.01f) * MMath::rotate(-10.0f, 1.0, 0.0, 0.0));
	//printf("current pos: %f %f %f\n", character->getPos().x, character->getPos().y, character->getPos().z);
}

void Scene01::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	/// Draw your scene here
	GLuint program = character->getShader()->getProgram();
	glUseProgram(program);

	/// These pass the matricies and the light position to the GPU
	glUniformMatrix4fv(character->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(character->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniform3fv(character->getShader()->getUniformID("lightPos"), 1, light1);

	//enemy and item renders
	if (roomUpdate == false) {
		if (hammer->getActive()) {
			hammer->Render();
		}
		if (turret1->isAlive()) {
			turret1->Render();
		}
		if (turret2->isAlive()) {
			turret2->Render();
		}
		if (turret3->isAlive()) {
			turret3->Render();
		}
		if (turret4->isAlive()) {
			turret4->Render();
		}
		if (turret5->isAlive()) {
			turret5->Render();
		}
		if (healthpot->getActive()) {
			healthpot->Render();
		}
	}
	//door and character renders
	if (character->getVisibility()) {
		character->Render();
	}
	//Room and UI renders
	healthBar->Render();
	wall1->Render();
	wall2->Render();
	wall3->Render();
	wall4->Render();
	floor->Render();
	doorLeft->Render();
	doorTop->Render();
	glUseProgram(0);
}

bool Scene01::setSceneCleared() {
	return roomCleared;
}

void Scene01::getSceneCleared(const bool storedRoom_) {
	roomCleared = storedRoom_;
}

float Scene01::setCharacterHealth() {
	return health;
}

void Scene01::getCharacterHealth(const float storedHealth_) {
	health = storedHealth_;
}

Vec3 Scene01::setCharacterPos() {
	return character->getPos();
}

void Scene01::getCharacterPos(const Vec3 storedPos_) {
	if (storedPos_.x >= 0 && storedPos_.y >= -1.0 && storedPos_.y <= 1.0) { //entering from left door
		returnedPos = Vec3((storedPos_.x * -1 + 1), storedPos_.y, storedPos_.z);
	}
	else if (storedPos_.x < 0 && storedPos_.y >= -1.0 && storedPos_.y <= 1.0) { //entering from right door
		returnedPos = Vec3((storedPos_.x * -1 - 1), storedPos_.y, storedPos_.z);
	}
	else if (storedPos_.y >= 0 && storedPos_.x >= -1.0 && storedPos_.x <= 1.0) { //entering from top door
		returnedPos = Vec3(storedPos_.x, (storedPos_.y * -1 + 1), storedPos_.z);
	}
	else if (storedPos_.y < 0 && storedPos_.x >= -1.0 && storedPos_.x <= 1.0) { //entering from bottom door
		returnedPos = Vec3(storedPos_.x, (storedPos_.y * -1 + -1), storedPos_.z);
	}
	else {
		returnedPos = Vec3((storedPos_.x * -1), (storedPos_.y * -1), storedPos_.z);
	}
}

float Scene01::setCharacterSpeed() {
	return character->getSpeed();
}

void Scene01::getCharacterSpeed(const float storedSpeed_) {
	speed = storedSpeed_;
}

int Scene01::setWeapon() {
	return weapon;
}

void Scene01::getWeapon(const int storedWeapon_) {
	weapon = storedWeapon_;
}

void Scene01::OnDestroy() {
	if (characterMesh) delete characterMesh, characterMesh = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (character) delete character, character = nullptr;
	if (healthBar) delete healthBar, healthBar = nullptr;
	if (wall1) delete wall1, wall1 = nullptr;
	if (wall2) delete wall2, wall2 = nullptr;
	if (wall3) delete wall3, wall3 = nullptr;
	if (wall4) delete wall4, wall4 = nullptr;
	if (floor) delete floor, floor = nullptr;
	if (doorLeft) delete doorLeft, doorLeft = nullptr;
	if (doorTop) delete doorTop, doorTop = nullptr;
}