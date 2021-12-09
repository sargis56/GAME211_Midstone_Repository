#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene0NEG1Under.h"
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

Scene0NEG1Under::Scene0NEG1Under() : character(nullptr), characterMesh(nullptr), shaderPtr(nullptr), texturePtr(nullptr), boxMesh(nullptr), doorLeft(nullptr), doorRight(nullptr), doorTexture(nullptr), floor(nullptr), floorTexture(nullptr),
health(NULL), healthBar(nullptr), healthUITexture(nullptr), wall1(nullptr), wall2(nullptr), wall3(nullptr), wall4(nullptr), wallTexture(nullptr), speed(NULL), weapon(NULL) {
	Debug::Info("Created SceneDev: ", __FILE__, __LINE__);
}

Scene0NEG1Under::~Scene0NEG1Under() {}

bool Scene0NEG1Under::OnCreate() {
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
	floor->setModelMatrix(MMath::translate(Vec3(0.0, 0.0, -17.0)) * MMath::scale(12.4f, 7.5f, 1.0f));
	doorLeft->setModelMatrix(MMath::translate(doorLeft->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f) * MMath::rotate(90, Vec3(0, 0, 1)));
	sword->setModelMatrix(MMath::translate(sword->getPos()) * MMath::scale(0.25f, 0.25f, 0.25f));
	doorRight->setModelMatrix(MMath::translate(doorRight->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f) * MMath::rotate(-90, Vec3(0, 0, 1)));
	return true;
}

void Scene0NEG1Under::HandleEvents(const SDL_Event& sdlEvent) {
	character->HandleEvents(sdlEvent);
}

int Scene0NEG1Under::SetScene() {
	return sceneNumber;
}

void Scene0NEG1Under::BuildCharacter() {
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

void Scene0NEG1Under::BuildAllEnemies() {
	ObjLoader::loadOBJ("meshes/Enemies/Mage.obj");
	mageMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	mageTexture = new Texture();
	mageTexture->LoadImage("textures/Enemies/Mage_D.png");
	mage = new MageEnemy(mageMesh, shaderPtr, mageTexture, room, character);
	mage->OnCreate();
	mage->setPos(Vec3(2.0f, 2.0f, -15.0f));
	ObjLoader::loadOBJ("meshes/Enemies/Demon.obj");
	demonMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	demonTexture = new Texture();
	demonTexture->LoadImage("textures/Enemies/Demon_Texture.jpg");
	demon1 = new DemonEnemy(demonMesh, shaderPtr, demonTexture, room, 10);
	demon1->setPos(Vec3(1.0f, -4.0f, -15.0f));
	demon2 = new DemonEnemy(demonMesh, shaderPtr, demonTexture, room, 10);
	demon2->setPos(Vec3(4.0f, 3.0f, -15.0f));
	demon3 = new DemonEnemy(demonMesh, shaderPtr, demonTexture, room, 10);
	demon3->setPos(Vec3(3.0f, 4.0f, -15.0f));

	ObjLoader::loadOBJ("meshes/Weapons/Sword.obj");
	swordMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	weaponTexture = new Texture();
	weaponTexture->LoadImage("textures/green.jpg");
	sword = new Sword(swordMesh, shaderPtr, weaponTexture, Vec3(-2.5f, -3.0f, -15.0f));
}

void Scene0NEG1Under::BuildRoom() {
	ObjLoader::loadOBJ("meshes/Cube.obj");
	boxMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	ObjLoader::loadOBJ("meshes/Scenery/DoorModel.obj");
	doorMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	wallTexture = new Texture();
	wallTexture->LoadImage("textures/underworldWall.jpg");
	floorTexture = new Texture();
	floorTexture->LoadImage("textures/underworldGround.jpg");
	doorTexture = new Texture();
	doorTexture->LoadImage("textures/Scenery/DoorModel_D.png");

	wall1 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall2 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall3 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall4 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	floor = new StaticMesh(boxMesh, shaderPtr, floorTexture);
	doorRight = new Door(doorMesh, shaderPtr, doorTexture, Vec3(9.5, 0.0, -15.0f));
	doorLeft = new Door(doorMesh, shaderPtr, doorTexture, Vec3(-9.5, 0.0, -15.0f));
}
void Scene0NEG1Under::BuildHealthUI() {
	healthUITexture = new Texture();
	healthUITexture->LoadImage("textures/red.jpg");
	healthBar = new HealthUI(boxMesh, shaderPtr, healthUITexture);
}

void Scene0NEG1Under::Update(const float deltaTime) {
	//enemy and item updates
	if (roomUpdate == false) {
		if (sword->getActive()) {
			sword->collisionCheck(character);
		}
		if (mage->isAlive()) {
			mage->Update(deltaTime);
			if (mage->getTimer() >= 20) {
				mageTexture->LoadImage("textures/Enemies/Mage_D.png");
				mage->ResetTimer();
			}
			if (mage->DamageCheck(character) && character->getInvincibility() == false && character->getAttacking() == false) {
				character->setinvincibilityTimer(100); //setting the timer for the invinciblity
				health -= 20; //set characters new health after taking damage
			}
			if (mage->WeaponColCheck(character) && character->getAttacking() == true) {
				//Enemy takes damage
				//ratEnemy->TakeDamage(character->getDamageFromPlayer());
				mage->TakeDamage(character->getDamageFromPlayer());
				mageTexture->LoadImage("textures/red.jpg");
				//printf("\nEnemy has taken damage");
			}
		}
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
		if (demon3->isAlive()) {
			demon3->Update(deltaTime);
			if (demon3->getTimer() >= 20) {
				demonTexture->LoadImage("textures/Enemies/Demon_Texture.jpg");
				demon3->ResetTimer();
			}
			if (demon3->DamageCheck(character) && character->getInvincibility() == false && character->getAttacking() == false) {
				character->setinvincibilityTimer(100); //setting the timer for the invinciblity
				health -= 15; //set characters new health after taking damage
			}
			if (demon3->WeaponColCheck(character) && character->getAttacking() == true) {
				//Enemy takes damage
				//ratEnemy->TakeDamage(character->getDamageFromPlayer());
				demon3->TakeDamage(character->getDamageFromPlayer());
				demonTexture->LoadImage("textures/red.jpg");
				//printf("\nEnemy has taken damage");
			}
		}
	}
	//door and character updates
	if (mage->isAlive() == false && demon1->isAlive() == false && demon2->isAlive() == false && demon3->isAlive() == false || roomCleared == true) { //enemies are dead - unlock room
		roomCleared = true;
		if (doorRight->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
			sceneNumber = 11;
		}
		if (doorLeft->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
			sceneNumber = 15;
		}
	}
	if (health <= 0) { //check if the player is dead
		sceneNumber = 31;
	}
	demon1->setModelMatrix(MMath::translate(demon1->getPos()) * MMath::rotate(demon1->FollowPlayer(character) + 90, Vec3(0.0f, 0.0f, 1.0f)));
	demon2->setModelMatrix(MMath::translate(demon2->getPos()) * MMath::rotate(demon2->FollowPlayer(character) + 90, Vec3(0.0f, 0.0f, 1.0f)));
	demon3->setModelMatrix(MMath::translate(demon3->getPos()) * MMath::rotate(demon3->FollowPlayer(character) + 90, Vec3(0.0f, 0.0f, 1.0f)));
	mage->setModelMatrix(MMath::translate(mage->getPos()) * MMath::rotate(mage->getRotation() + 90, Vec3(0.0f, 0.0f, 1.0f)) * MMath::scale(Vec3(0.5, 0.5, 0.5f)));
	character->checkInvincibility(); //checking if the character is invincible
	character->setModelMatrix(MMath::translate(character->getPos()) * MMath::rotate(character->getRotation(), Vec3(0.0f, 0.0f, 1.0f)));
	healthBar->setModelMatrix(MMath::translate(Vec3(0.0f, -3.5f, -5.0f)) * MMath::scale(0.05f * (health + 0.01), 0.3f, 0.01f) * MMath::rotate(-10.0f, 1.0, 0.0, 0.0));
	//printf("current pos: %f %f %f\n", character->getPos().x, character->getPos().y, character->getPos().z);
}

void Scene0NEG1Under::Render() const {
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
		if (demon3->isAlive()) {
			demon3->Render();
		}
		if (mage->isAlive()) {
			mage->Render();
		}
		if (sword->getActive()) {
			sword->Render();
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
	doorRight->Render();
	glUseProgram(0);
}

bool Scene0NEG1Under::setSceneCleared() {
	return roomCleared;
}

void Scene0NEG1Under::getSceneCleared(const bool storedRoom_) {
	roomCleared = storedRoom_;
}

float Scene0NEG1Under::setCharacterHealth() {
	return health;
}

void Scene0NEG1Under::getCharacterHealth(const float storedHealth_) {
	health = storedHealth_;
}

Vec3 Scene0NEG1Under::setCharacterPos() {
	return character->getPos();
}

void Scene0NEG1Under::getCharacterPos(const Vec3 storedPos_) {
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

float Scene0NEG1Under::setCharacterSpeed() {
	return character->getSpeed();
}

void Scene0NEG1Under::getCharacterSpeed(const float storedSpeed_) {
	speed = storedSpeed_;
}

int Scene0NEG1Under::setWeapon() {
	return weapon;
}

void Scene0NEG1Under::getWeapon(const int storedWeapon_) {
	weapon = storedWeapon_;
}

void Scene0NEG1Under::OnDestroy() {
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
	if (doorRight) delete doorRight, doorRight = nullptr;
}