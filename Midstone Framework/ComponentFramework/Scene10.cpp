#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene10.h"
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

Scene10::Scene10() : character(nullptr), characterMesh(nullptr), shaderPtr(nullptr), texturePtr(nullptr), boxMesh(nullptr), doorLeft(nullptr), doorBottom(nullptr), doorRight(nullptr), doorTop(nullptr), doorTexture(nullptr), floor(nullptr), floorTexture(nullptr),
health(NULL), healthBar(nullptr), healthUITexture(nullptr), wall1(nullptr), wall2(nullptr), wall3(nullptr), wall4(nullptr), wallTexture(nullptr), speed(NULL), weapon(NULL) {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene10::~Scene10() {}

bool Scene10::OnCreate() {
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
	doorRight->setModelMatrix(MMath::translate(doorRight->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f) * MMath::rotate(-90, Vec3(0, 0, 1)));
	doorBottom->setModelMatrix(MMath::translate(doorBottom->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f) * MMath::rotate(180, Vec3(0, 0, 1)));
	healthpot->setModelMatrix(MMath::translate(healthpot->getPos()) * MMath::scale(0.7f, 0.7f, 0.7f));
	return true;
}

void Scene10::HandleEvents(const SDL_Event& sdlEvent) {
	character->HandleEvents(sdlEvent);
}

int Scene10::SetScene() {
	return sceneNumber;
}

void Scene10::BuildCharacter() {
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

void Scene10::BuildAllEnemies() {
	ObjLoader::loadOBJ("meshes/Enemies/Rat.obj");
	ratMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	ratTexture = new Texture();
	ratTexture->LoadImage("textures/Enemies/Rat_Texture.jpg");
	ratEnemy = new RatEnemy(ratMesh, shaderPtr, ratTexture, room);
	ratEnemy->setPos(Vec3(0.0f,2.0f,-15.0f));

	ObjLoader::loadOBJ("meshes/Items/Potion.obj");
	healthPotMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	healthPotTexture = new Texture();
	healthPotTexture->LoadImage("textures/green.jpg");
	healthpot = new HealingItem(healthPotMesh, shaderPtr, healthPotTexture, Vec3(5.0,3.0,-15.0));
}

void Scene10::BuildRoom() {
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
	doorRight = new Door(doorMesh, shaderPtr, doorTexture, Vec3(9.5, 0.0, -15.0f));
	doorLeft = new Door(doorMesh, shaderPtr, doorTexture, Vec3(-9.5, 0.0, -15.0f));
	doorTop = new Door(doorMesh, shaderPtr, doorTexture, Vec3(0.0, 4.5, -15.0f));
	doorBottom = new Door(doorMesh, shaderPtr, doorTexture, Vec3(0.0, -4.5, -15.0f));
}
void Scene10::BuildHealthUI() {
	healthUITexture = new Texture();
	healthUITexture->LoadImage("textures/red.jpg");
	healthBar = new HealthUI(boxMesh, shaderPtr, healthUITexture);
}

void Scene10::Update(const float deltaTime) {
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
			sceneNumber = 9;
		}
		if (doorBottom->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
			sceneNumber = 1;
		}
		if (doorRight->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
			sceneNumber = 7;
		}
		if (doorLeft->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
			sceneNumber = 4;
		}
	}
	ratEnemy->setModelMatrix(MMath::translate(ratEnemy->getPos()) * MMath::rotate(ratEnemy->getRotation(), Vec3(0,0,1)));
	//character updates
	character->checkInvincibility(); //checking if the character is invincible
	character->setModelMatrix(MMath::translate(character->getPos()) * MMath::rotate(character->getRotation(), Vec3(0.0f, 0.0f, 1.0f)));
	healthBar->setModelMatrix(MMath::translate(Vec3(0.0f, -3.5f, -5.0f)) * MMath::scale(0.05f * (health + 0.01), 0.3f, 0.01f) * MMath::rotate(-10.0f, 1.0, 0.0, 0.0));
	//printf("current pos: %f %f %f\n", character->getPos().x, character->getPos().y, character->getPos().z);
}

void Scene10::Render() const {
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
	doorLeft->Render();
	doorTop->Render();
	doorRight->Render();
	glUseProgram(0);
}

bool Scene10::setSceneCleared() {
	return roomCleared;
}

void Scene10::getSceneCleared(const bool storedRoom_) {
	roomCleared = storedRoom_;
}

float Scene10::setCharacterHealth() {
	return health;
}

void Scene10::getCharacterHealth(const float storedHealth_) {
	health = storedHealth_;
}

Vec3 Scene10::setCharacterPos() {
	return character->getPos();
}

void Scene10::getCharacterPos(const Vec3 storedPos_) {
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

float Scene10::setCharacterSpeed() {
	return character->getSpeed();
}

void Scene10::getCharacterSpeed(const float storedSpeed_) {
	speed = storedSpeed_;
}

int Scene10::setWeapon() {
	return weapon;
}

void Scene10::getWeapon(const int storedWeapon_) {
	weapon = storedWeapon_;
}

void Scene10::OnDestroy() {
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
	if (doorBottom) delete doorBottom, doorBottom = nullptr;
	if (doorRight) delete doorRight, doorRight = nullptr;
	if (doorTop) delete doorBottom, doorBottom = nullptr;
	if (ratEnemy) delete ratEnemy, ratEnemy = nullptr;
}