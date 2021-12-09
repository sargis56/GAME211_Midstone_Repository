#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene10Under.h"
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

Scene10Under::Scene10Under() : character(nullptr), characterMesh(nullptr), shaderPtr(nullptr), texturePtr(nullptr), boxMesh(nullptr), doorLeft(nullptr), doorBottom(nullptr), doorRight(nullptr), doorTop(nullptr), doorTexture(nullptr), floor(nullptr), floorTexture(nullptr),
health(NULL), healthBar(nullptr), healthUITexture(nullptr), wall1(nullptr), wall2(nullptr), wall3(nullptr), wall4(nullptr), wallTexture(nullptr), speed(NULL), weapon(NULL) {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene10Under::~Scene10Under() {}

bool Scene10Under::OnCreate() {
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
	maul->setModelMatrix(MMath::translate(maul->getPos()) * MMath::scale(0.25f, 0.25f, 0.25f));
	doorBottom->setModelMatrix(MMath::translate(doorBottom->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f) * MMath::rotate(180, Vec3(0, 0, 1)));
	return true;
}

void Scene10Under::HandleEvents(const SDL_Event& sdlEvent) {
	character->HandleEvents(sdlEvent);
}

int Scene10Under::SetScene() {
	return sceneNumber;
}

void Scene10Under::BuildCharacter() {
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

void Scene10Under::BuildAllEnemies() {
	ObjLoader::loadOBJ("meshes/Enemies/Archer.obj");
	archerMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	archerTexture = new Texture();
	archerTexture->LoadImage("textures/Enemies/Archer_D.png");
	archer1 = new ArcherEnemy(archerMesh, shaderPtr, archerTexture, room, character);
	archer2 = new ArcherEnemy(archerMesh, shaderPtr, archerTexture, room, character);
	archer1->OnCreate();
	archer2->OnCreate();
	ObjLoader::loadOBJ("meshes/Enemies/MagicTurret.obj");
	turretMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	turretTexture = new Texture();
	turretTexture->LoadImage("textures/Enemies/Turret_Texture.jpg");
	turret = new MageTurretEnemy(turretMesh, shaderPtr, turretTexture, room);
	turret->OnCreate();
	turret->setPos(Vec3(0.0f, 1.0f, -15.0f));
	archer1->setPos(Vec3(-2.0f, 4.0f, -15.0f));
	archer2->setPos(Vec3(2.0f, -4.0f, -15.0f));

	ObjLoader::loadOBJ("meshes/Weapons/Maul.obj");
	maulMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	weaponTexture = new Texture();
	weaponTexture->LoadImage("textures/green.jpg");
	maul = new Maul(maulMesh, shaderPtr, weaponTexture, Vec3(-7.5f, 0.0f, -15.0f));
}

void Scene10Under::BuildRoom() {
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
	doorTop = new Door(doorMesh, shaderPtr, doorTexture, Vec3(0.0, 4.5, -15.0f));
	doorBottom = new Door(doorMesh, shaderPtr, doorTexture, Vec3(0.0, -4.5, -15.0f));
}
void Scene10Under::BuildHealthUI() {
	healthUITexture = new Texture();
	healthUITexture->LoadImage("textures/red.jpg");
	healthBar = new HealthUI(boxMesh, shaderPtr, healthUITexture);
}

void Scene10Under::Update(const float deltaTime) {
	//enemy and item updates
	if (roomUpdate == false) {
		if (maul->getActive()) {
			maul->collisionCheck(character);
		}
		if (archer1->isAlive()) {
			archer1->Update(deltaTime);
			if (archer1->getTimer() >= 20) {
				archerTexture->LoadImage("textures/Enemies/Archer_D.png");
				archer1->ResetTimer();
			}
			if (archer1->DamageCheck(character) && character->getInvincibility() == false && character->getAttacking() == false) {
				character->setinvincibilityTimer(100); //setting the timer for the invinciblity
				health -= 20; //set characters new health after taking damage
			}
			if (archer1->WeaponColCheck(character) && character->getAttacking() == true) {
				//Enemy takes damage
				//ratEnemy->TakeDamage(character->getDamageFromPlayer());
				archer1->TakeDamage(character->getDamageFromPlayer());
				archerTexture->LoadImage("textures/red.jpg");
				//printf("\nEnemy has taken damage");
			}
		}
		if (archer2->isAlive()) {
			archer2->Update(deltaTime);
			if (archer2->getTimer() >= 20) {
				archerTexture->LoadImage("textures/Enemies/Archer_D.png");
				archer2->ResetTimer();
			}
			if (archer2->DamageCheck(character) && character->getInvincibility() == false && character->getAttacking() == false) {
				character->setinvincibilityTimer(100); //setting the timer for the invinciblity
				health -= 20; //set characters new health after taking damage
			}
			if (archer2->WeaponColCheck(character) && character->getAttacking() == true) {
				//Enemy takes damage
				//ratEnemy->TakeDamage(character->getDamageFromPlayer());
				archer2->TakeDamage(character->getDamageFromPlayer());
				archerTexture->LoadImage("textures/red.jpg");
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
	}
	//door and character updates
	if (archer1->isAlive() == false && archer2->isAlive() == false && turret->isAlive() == false || roomCleared == true) { //enemies are dead - unlock room
		roomCleared = true;
		if (doorTop->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
			sceneNumber = 19;
		}
		if (doorBottom->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
			sceneNumber = 11;
		}
		if (doorRight->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
			sceneNumber = 17;
		}
		if (doorLeft->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
			sceneNumber = 14;
		}
	}
	if (health <= 0) { //check if the player is dead
		sceneNumber = 31;
	}
	turret->setModelMatrix(MMath::translate(turret->getPos()) * MMath::scale(Vec3(0.5, 0.5, 0.5f)));
	archer1->setModelMatrix(MMath::translate(archer1->getPos()) * MMath::rotate(archer1->getRotation() + 90, Vec3(0.0f, 0.0f, 1.0f)) * MMath::scale(Vec3(0.5, 0.5, 0.5f)));
	archer2->setModelMatrix(MMath::translate(archer2->getPos()) * MMath::rotate(archer2->getRotation() + 90, Vec3(0.0f, 0.0f, 1.0f)) * MMath::scale(Vec3(0.5, 0.5, 0.5f)));
	character->checkInvincibility(); //checking if the character is invincible
	character->setModelMatrix(MMath::translate(character->getPos()) * MMath::rotate(character->getRotation(), Vec3(0.0f, 0.0f, 1.0f)));
	healthBar->setModelMatrix(MMath::translate(Vec3(0.0f, -3.5f, -5.0f)) * MMath::scale(0.05f * (health + 0.01), 0.3f, 0.01f) * MMath::rotate(-10.0f, 1.0, 0.0, 0.0));
	//printf("current pos: %f %f %f\n", character->getPos().x, character->getPos().y, character->getPos().z);
}

void Scene10Under::Render() const {
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
		if (archer1->isAlive()) {
			archer1->Render();
		}
		if (archer2->isAlive()) {
			archer2->Render();
		}
		if (turret->isAlive()) {
			turret->Render();
		}
		if (maul->getActive()) {
			maul->Render();
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

bool Scene10Under::setSceneCleared() {
	return roomCleared;
}

void Scene10Under::getSceneCleared(const bool storedRoom_) {
	roomCleared = storedRoom_;
}

float Scene10Under::setCharacterHealth() {
	return health;
}

void Scene10Under::getCharacterHealth(const float storedHealth_) {
	health = storedHealth_;
}

Vec3 Scene10Under::setCharacterPos() {
	return character->getPos();
}

void Scene10Under::getCharacterPos(const Vec3 storedPos_) {
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

float Scene10Under::setCharacterSpeed() {
	return character->getSpeed();
}

void Scene10Under::getCharacterSpeed(const float storedSpeed_) {
	speed = storedSpeed_;
}

int Scene10Under::setWeapon() {
	return weapon;
}

void Scene10Under::getWeapon(const int storedWeapon_) {
	weapon = storedWeapon_;
}

void Scene10Under::OnDestroy() {
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
	if (doorTop) delete doorTop, doorTop = nullptr;
}