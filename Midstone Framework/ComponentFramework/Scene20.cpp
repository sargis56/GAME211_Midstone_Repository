#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene20.h"
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

Scene20::Scene20() : character(nullptr), characterMesh(nullptr), shaderPtr(nullptr), texturePtr(nullptr), boxMesh(nullptr), doorBottom(nullptr), doorTop(nullptr), doorGoldTexture(nullptr), doorTexture(nullptr), floor(nullptr), floorTexture(nullptr),
health(NULL), healthBar(nullptr), healthUITexture(nullptr), wall1(nullptr), wall2(nullptr), wall3(nullptr), wall4(nullptr), wallTexture(nullptr), speed(NULL), weapon(NULL), doorMesh(nullptr){
	//Debug::Info("Created SceneDev: ", __FILE__, __LINE__);
}

Scene20::~Scene20() {}

bool Scene20::OnCreate() {
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
	doorTop->setModelMatrix(MMath::translate(doorTop->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f));
	spear->setModelMatrix(MMath::translate(spear->getPos()) * MMath::scale(0.25f, 0.25f, 0.25f));
	doorBottom->setModelMatrix(MMath::translate(doorBottom->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f) * MMath::rotate(180, Vec3(0, 0, 1)));
	return true;
}

void Scene20::HandleEvents(const SDL_Event& sdlEvent) {
	character->HandleEvents(sdlEvent);
}

int Scene20::SetScene() {
	return sceneNumber;
}

void Scene20::BuildCharacter() {
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

void Scene20::BuildAllEnemies() {
	ObjLoader::loadOBJ("meshes/Enemies/Demon.obj");
	demonMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	demonTexture = new Texture();
	demonTexture->LoadImage("textures/Enemies/Demon_Texture.jpg");
	demon1 = new DemonEnemy(demonMesh, shaderPtr, demonTexture, room, 10);
	demon1->setPos(Vec3(-2.0f, -3.0f, -15.0f));
	demon2 = new DemonEnemy(demonMesh, shaderPtr, demonTexture, room, 10);
	demon2->setPos(Vec3(2.0f, 3.0f, -15.0f));
	ObjLoader::loadOBJ("meshes/Enemies/MagicTurret.obj");
	turretMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	turretTexture = new Texture();
	turretTexture->LoadImage("textures/Enemies/Turret_Texture.jpg");
	turret = new MageTurretEnemy(turretMesh, shaderPtr, turretTexture, room);
	turret->OnCreate();
	turret->setPos(Vec3(0, 0, -15));

	ObjLoader::loadOBJ("meshes/Weapons/Spear.obj");
	spearMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	weaponTexture = new Texture();
	weaponTexture->LoadImage("textures/green.jpg");
	spear = new Spear(spearMesh, shaderPtr, weaponTexture, Vec3(-5.0f, 2.5f, -15.0f));
}

void Scene20::BuildRoom() {
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
	doorGoldTexture = new Texture();
	doorGoldTexture->LoadImage("textures/Scenery/DoorModel_D_Gold.png");

	wall1 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall2 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall3 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall4 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	floor = new StaticMesh(boxMesh, shaderPtr, floorTexture);
	doorTop = new Door(doorMesh, shaderPtr, doorGoldTexture, Vec3(0.0, 4.5, -15.0f));
	doorBottom = new Door(doorMesh, shaderPtr, doorTexture, Vec3(0.0, -4.5, -15.0f));
}
void Scene20::BuildHealthUI() {
	healthUITexture = new Texture();
	healthUITexture->LoadImage("textures/red.jpg");
	healthBar = new HealthUI(boxMesh, shaderPtr, healthUITexture);
}

void Scene20::Update(const float deltaTime) {
	//enemy and item updates
	if (roomUpdate == false) {
		if (demon1->isAlive()) {
			demon1->Update(deltaTime);
			if (demon1->getTimer() >= 20) {
				demonTexture->LoadImage("textures/Enemies/Demon_Texture.jpg");
				demon1->ResetTimer();
			}
			if (demon1->DamageCheck(character) && character->getInvincibility() == false && character->getAttacking() == false) {
				character->setinvincibilityTimer(100); //setting the timer for the invinciblity
				health -= 15; //set characters new health after taking damage
			}
			if (demon1->WeaponColCheck(character) && character->getAttacking() == true) {
				//Enemy takes damage
				//ratEnemy->TakeDamage(character->getDamageFromPlayer());
				demon1->TakeDamage(character->getDamageFromPlayer());
				demonTexture->LoadImage("textures/red.jpg");
				//printf("\nEnemy has taken damage");
			}
		}
		if (demon2->isAlive()) {
			demon2->Update(deltaTime);
			if (demon2->getTimer() >= 20) {
				demonTexture->LoadImage("textures/Enemies/Demon_Texture.jpg");
				demon2->ResetTimer();
			}
			if (demon2->DamageCheck(character) && character->getInvincibility() == false && character->getAttacking() == false) {
				character->setinvincibilityTimer(100); //setting the timer for the invinciblity
				health -= 15; //set characters new health after taking damage
			}
			if (demon2->WeaponColCheck(character) && character->getAttacking() == true) {
				//Enemy takes damage
				//ratEnemy->TakeDamage(character->getDamageFromPlayer());
				demon2->TakeDamage(character->getDamageFromPlayer());
				demonTexture->LoadImage("textures/red.jpg");
				//printf("\nEnemy has taken damage");
			}
		}
		if (turret->isAlive()) {
			turret->Update(deltaTime);
			if (turret->getTimer() >= 20) {
				turretTexture->LoadImage("textures/Enemies/Turret_Texture.jpg");
				turret->ResetTimer();
			}
			if (turret->DamageCheck(character) && character->getInvincibility() == false && character->getAttacking() == false) {
				character->setinvincibilityTimer(100); //setting the timer for the invinciblity
				health -= 20; //set characters new health after taking damage
			}
			if (turret->WeaponColCheck(character) && character->getAttacking() == true) {
				//Enemy takes damage
				//ratEnemy->TakeDamage(character->getDamageFromPlayer());
				turret->TakeDamage(character->getDamageFromPlayer());
				turretTexture->LoadImage("textures/red.jpg");
				//printf("\nEnemy has taken damage");
			}
		}
		if (spear->getActive()) {
			spear->collisionCheck(character);
		}
	}
	//door and character updates
	if (demon1->isAlive() == false && demon2->isAlive() == false && turret->isAlive() == false || roomCleared == true) { //enemies are dead - unlock room
		roomCleared = true;
		if (doorTop->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
			sceneNumber = 20;
		}
		if (doorBottom->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
			sceneNumber = 2;
		}
	}
	if (health <= 0) { //check if the player is dead
		sceneNumber = 31;
	}
	demon1->setModelMatrix(MMath::translate(demon1->getPos()) * MMath::rotate(demon1->FollowPlayer(character) + 90, Vec3(0.0f, 0.0f, 1.0f)));
	demon2->setModelMatrix(MMath::translate(demon2->getPos()) * MMath::rotate(demon2->FollowPlayer(character) + 90, Vec3(0.0f, 0.0f, 1.0f)));
	turret->setModelMatrix(MMath::translate(turret->getPos()) * MMath::scale(Vec3(0.5, 0.5, 0.5f)));
	character->checkInvincibility(); //checking if the character is invincible
	character->setModelMatrix(MMath::translate(character->getPos()) * MMath::rotate(character->getRotation(), Vec3(0.0f, 0.0f, 1.0f)));
	healthBar->setModelMatrix(MMath::translate(Vec3(0.0f, -3.5f, -5.0f)) * MMath::scale(0.05f * (health + 0.01), 0.3f, 0.01f) * MMath::rotate(-10.0f, 1.0, 0.0, 0.0));
	//printf("current pos: %f %f %f\n", character->getPos().x, character->getPos().y, character->getPos().z);
}

void Scene20::Render() const {
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
		if (demon1->isAlive()) {
			demon1->Render();
		}
		if (demon2->isAlive()) {
			demon2->Render();
		}
		if (turret->isAlive()) {
			turret->Render();
		}
		if (spear->getActive()) {
			spear->Render();
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
	doorBottom->Render();
	doorTop->Render();
	glUseProgram(0);
}

bool Scene20::setSceneCleared() {
	return roomCleared;
}

void Scene20::getSceneCleared(const bool storedRoom_) {
	roomCleared = storedRoom_;
}

float Scene20::setCharacterHealth() {
	return health;
}

void Scene20::getCharacterHealth(const float storedHealth_) {
	health = storedHealth_;
}

Vec3 Scene20::setCharacterPos() {
	return character->getPos();
}

void Scene20::getCharacterPos(const Vec3 storedPos_) {
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

float Scene20::setCharacterSpeed() {
	return character->getSpeed();
}

void Scene20::getCharacterSpeed(const float storedSpeed_) {
	speed = storedSpeed_;
}

int Scene20::setWeapon() {
	return weapon;
}

void Scene20::getWeapon(const int storedWeapon_) {
	weapon = storedWeapon_;
}

void Scene20::OnDestroy() {
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
	if (doorBottom) delete doorBottom, doorBottom = nullptr;
	if (doorTop) delete doorTop, doorTop = nullptr;

}