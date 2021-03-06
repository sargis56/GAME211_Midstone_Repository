#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "SceneNEG11Under.h"
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

SceneNEG11Under::SceneNEG11Under() : character(nullptr), characterMesh(nullptr), shaderPtr(nullptr), texturePtr(nullptr), boxMesh(nullptr), doorTop(nullptr), doorTexture(nullptr), floor(nullptr), floorTexture(nullptr),
health(NULL), healthBar(nullptr), healthUITexture(nullptr), wall1(nullptr), wall2(nullptr), wall3(nullptr), wall4(nullptr), wallTexture(nullptr), speed(NULL), weapon(NULL) {
	//Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

SceneNEG11Under::~SceneNEG11Under() {}

bool SceneNEG11Under::OnCreate() {
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
	axe->setModelMatrix(MMath::translate(axe->getPos()) * MMath::scale(0.25f, 0.25f, 0.25f));
	healthpot->setModelMatrix(MMath::translate(healthpot->getPos()) * MMath::scale(0.7f, 0.7f, 0.7f));
	return true;
}

void SceneNEG11Under::HandleEvents(const SDL_Event& sdlEvent) {
	character->HandleEvents(sdlEvent);
}

int SceneNEG11Under::SetScene() {
	return sceneNumber;
}

void SceneNEG11Under::BuildCharacter() {
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

void SceneNEG11Under::BuildAllEnemies() {
	ObjLoader::loadOBJ("meshes/Enemies/Rat.obj");
	ratMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	ratTexture = new Texture();
	ratTexture->LoadImage("textures/Enemies/Rat_Texture.jpg");
	ratEnemy = new RatEnemy(ratMesh, shaderPtr, ratTexture, room);
	ratEnemy->setPos(Vec3(0.0f,-2.0f,-15.0f));

	ObjLoader::loadOBJ("meshes/Items/Potion.obj");
	healthPotMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	healthPotTexture = new Texture();
	healthPotTexture->LoadImage("textures/green.jpg");
	healthpot = new HealingItem(healthPotMesh, shaderPtr, healthPotTexture, Vec3(5.0,3.0,-15.0));

	ObjLoader::loadOBJ("meshes/Weapons/Axe.obj");
	axeMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	weaponTexture = new Texture();
	weaponTexture->LoadImage("textures/green.jpg");
	axe = new Axe(axeMesh, shaderPtr, weaponTexture, Vec3(-5.0f, -3.0f, -15.0f));
}

void SceneNEG11Under::BuildRoom() {
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
	doorTop = new Door(doorMesh, shaderPtr, doorTexture, Vec3(0.0, 4.5, -15.0f));
}
void SceneNEG11Under::BuildHealthUI() {
	healthUITexture = new Texture();
	healthUITexture->LoadImage("textures/red.jpg");
	healthBar = new HealthUI(boxMesh, shaderPtr, healthUITexture);
}

void SceneNEG11Under::Update(const float deltaTime) {
	//enemy and item updates
	if (roomUpdate == false) {
		if (ratEnemy->isAlive()) {
			ratEnemy->Update(deltaTime);
			if (ratEnemy->getTimer() >= 20) {
				ratTexture->LoadImage("textures/Enemies/Rat_Texture.jpg");
				ratEnemy->ResetTimer();
			}
			if (ratEnemy->DamageCheck(character) && character->getInvincibility() == false && character->getAttacking() == false) {
				character->setinvincibilityTimer(100); //setting the timer for the invinciblity
				health -= 15; //set characters new health after taking damage
			}
			if (ratEnemy->WeaponColCheck(character) && character->getAttacking() == true) {
				//Enemy takes damage
				//ratEnemy->TakeDamage(character->getDamageFromPlayer());
				ratEnemy->TakeDamage(character->getDamageFromPlayer());
				ratTexture->LoadImage("textures/red.jpg");
				//printf("\nEnemy has taken damage");
			}
		}
		if (axe->getActive()) {
			axe->collisionCheck(character);
		}
		if (health <= 0) { //check if the player is dead
			sceneNumber = 31;
		}
		if (healthpot->getActive() && healthpot->collisionCheck(character)) {
			health = health + 20;
			if (health > 50) {
				health = 50;
			}
		}
	}
	if (ratEnemy->isAlive() == false || roomCleared == true) { //enemies are dead - unlock room
		roomCleared = true;
		//door updates
		if (doorTop->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
			sceneNumber = 16;
		}
	}
	ratEnemy->setModelMatrix(MMath::translate(ratEnemy->getPos()) * MMath::rotate(ratEnemy->getRotation(), Vec3(0,0,1)));
	//character updates
	character->checkInvincibility(); //checking if the character is invincible
	character->setModelMatrix(MMath::translate(character->getPos()) * MMath::rotate(character->getRotation(), Vec3(0.0f, 0.0f, 1.0f)));
	healthBar->setModelMatrix(MMath::translate(Vec3(0.0f, -3.5f, -5.0f)) * MMath::scale(0.05f * (health + 0.01), 0.3f, 0.01f) * MMath::rotate(-10.0f, 1.0, 0.0, 0.0));
	//printf("current pos: %f %f %f\n", character->getPos().x, character->getPos().y, character->getPos().z);
}

void SceneNEG11Under::Render() const {
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
		if (ratEnemy->isAlive()) {
			ratEnemy->Render();
		}
		if (healthpot->getActive()) {
			healthpot->Render();
		}
		if (axe->getActive()) {
			axe->Render();
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
	doorTop->Render();
	glUseProgram(0);
}

bool SceneNEG11Under::setSceneCleared() {
	return roomCleared;
}

void SceneNEG11Under::getSceneCleared(const bool storedRoom_) {
	roomCleared = storedRoom_;
}

float SceneNEG11Under::setCharacterHealth() {
	return health;
}

void SceneNEG11Under::getCharacterHealth(const float storedHealth_) {
	health = storedHealth_;
}

Vec3 SceneNEG11Under::setCharacterPos() {
	return character->getPos();
}

void SceneNEG11Under::getCharacterPos(const Vec3 storedPos_) {
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

float SceneNEG11Under::setCharacterSpeed() {
	return character->getSpeed();
}

void SceneNEG11Under::getCharacterSpeed(const float storedSpeed_) {
	speed = storedSpeed_;
}

int SceneNEG11Under::setWeapon() {
	return weapon;
}

void SceneNEG11Under::getWeapon(const int storedWeapon_) {
	weapon = storedWeapon_;
}

void SceneNEG11Under::OnDestroy() {
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
	if (doorTop) delete doorTop, doorTop = nullptr;
	if (ratEnemy) delete ratEnemy, ratEnemy = nullptr;
}