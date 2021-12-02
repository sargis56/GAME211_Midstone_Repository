#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "SceneTest.h"
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

SceneTest::SceneTest() : character(nullptr), meshPtr(nullptr), shaderPtr(nullptr), snakeMeshPtr(nullptr), snakeTexture(nullptr), texturePtr(nullptr), boxMesh(nullptr), doorTexture(nullptr), enemy1(nullptr), floor(nullptr), floorTexture(nullptr),
health(NULL), healthBar(nullptr), healthUITexture(nullptr), ratMeshPtr(nullptr), turretTexture(nullptr), wall1(nullptr), wall2(nullptr), wall3(nullptr), wall4(nullptr), wallTexture(nullptr), speedItem(nullptr) {
	Debug::Info("Created SceneTest: ", __FILE__, __LINE__);
}

SceneTest::~SceneTest() {}

bool SceneTest::OnCreate() {
	health = 50;
	//Room setup
	room.setRoomBorders(Vec3(-9.0, -4.0, 0.0), Vec3(9.0, 4.0, 0.0));
	light1 = Vec3(0.0f, 40.0f, 0.0f);
	roomUpdate = roomCleared; //setting local clear
	projectionMatrix = MMath::perspective(30.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	//character onCreate
	BuildCharacter();
	character = new Character(meshPtr, shaderPtr, texturePtr, room);
	character->setSpeed(speed);
	if (character == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	//room onCreate
	BuildRoom();
	wall1 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall2 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall3 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall4 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	floor = new StaticMesh(boxMesh, shaderPtr, floorTexture);
	BuildHealthUI();
	healthBar = new HealthUI(boxMesh, shaderPtr, healthUITexture);
	speedItem = new SpeedItem(boxMesh, shaderPtr, doorTexture, 0.2f, Vec3(3.0f, 3.0f, -15.0f));
	character->setPos(returnedPos); //using for setting the position 
	//enemy onCreate
	BuildAllEnemies();
	enemy1 = new FinalEnemy(ratMeshPtr, shaderPtr, turretTexture, room, character, 0);
	enemy1->OnCreate();
	enemy1->setPos(Vec3(2.0, 2.0, -15.0));
	healingItem = new HealingItem(itemMesh, shaderPtr, doorTexture, Vec3(3.0f, -3.0f, -15.0f));
	shovel = new Shovel(shovelMesh, shaderPtr, doorTexture, Vec3(0.0f, -3.0f, -15.0f));
	sword = new Sword(swordMesh, shaderPtr, doorTexture, Vec3(-2.5f, -3.0f, -15.0f));
	axe = new Axe(axeMesh, shaderPtr, doorTexture, Vec3(-5.0f, -3.0f, -15.0f));
	zwei = new Zweihander(zweiMesh, shaderPtr, doorTexture, Vec3(-7.5f, -3.0f, -15.0f));

	//snakeEnemy = new SnakeEnemy(snakeMeshPtr, shaderPtr, snakeTexture, room);
	//setting modelMatrix for static objs
	healthBar->setModelMatrix(MMath::translate(Vec3(0.0f, -3.5f, -5.0f)) * MMath::scale(0.05f * (health + 0.01), 0.3f, 0.01f) * MMath::rotate(-10.0f, 1.0, 0.0, 0.0));
	wall1->setModelMatrix(MMath::translate(Vec3(-11.0, 0.0, -15.0)) * MMath::scale(0.75f, 5.0f, 1.0f));
	wall2->setModelMatrix(MMath::translate(Vec3(11.0, 0.0, -15.0)) * MMath::scale(0.75f, 5.0f, 1.0f));
	wall3->setModelMatrix(MMath::translate(Vec3(0.0, -5.75, -15.0)) * MMath::scale(11.5f, 0.75f, 1.0f));
	wall4->setModelMatrix(MMath::translate(Vec3(0.0, 5.75, -15.0)) * MMath::scale(11.5f, 0.75f, 1.0f));
	floor->setModelMatrix(MMath::translate(Vec3(0.0, 0.0, -17.0)) * MMath::scale(11.4f, 5.5f, 1.0f));
	speedItem->setModelMatrix(MMath::translate(speedItem->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f));
	healingItem->setModelMatrix(MMath::translate(healingItem->getPos()) * MMath::scale(0.7f, 0.7f, 0.7f));
	shovel->setModelMatrix(MMath::translate(shovel->getPos()) * MMath::scale(0.25f, 0.25f, 0.25f));
	sword->setModelMatrix(MMath::translate(sword->getPos()) * MMath::scale(0.25f, 0.25f, 0.25f));
	axe->setModelMatrix(MMath::translate(axe->getPos()) * MMath::scale(0.25f, 0.25f, 0.25f));
	zwei->setModelMatrix(MMath::translate(zwei->getPos()) * MMath::scale(0.25f, 0.25f, 0.25f));

	return true;
}

void SceneTest::HandleEvents(const SDL_Event& sdlEvent) {
	character->HandleEvents(sdlEvent);
}

int SceneTest::SetScene() {
	return sceneNumber;
}

void SceneTest::BuildCharacter() {
	ObjLoader::loadOBJ("meshes/Characters/Pl_LordWill.obj");
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	texturePtr = new Texture();
	texturePtr->LoadImage("textures/Characters/Pl_LordW.png");
}

void SceneTest::BuildAllEnemies() {
	ObjLoader::loadOBJ("meshes/Enemies/MagicTurret.obj");
	ratMeshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	turretTexture = new Texture();
	turretTexture->LoadImage("textures/Enemies/Turret_Texture.jpg");

	ObjLoader::loadOBJ("meshes/Items/Potion.obj");
	itemMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	ObjLoader::loadOBJ("meshes/Weapons/Sword.obj");
	swordMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	ObjLoader::loadOBJ("meshes/Weapons/Shovel.obj");
	shovelMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	ObjLoader::loadOBJ("meshes/Weapons/Axe.obj");
	axeMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	ObjLoader::loadOBJ("meshes/Weapons/Zwei.obj");
	zweiMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	ObjLoader::loadOBJ("meshes/Enemies/Rat.obj");
	snakeMeshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	snakeTexture = new Texture();
	snakeTexture->LoadImage("textures/Enemies/Rat_Texture.jpg");
	
	
}

void SceneTest::BuildRoom() {
	ObjLoader::loadOBJ("meshes/Cube.obj");
	boxMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	wallTexture = new Texture();
	wallTexture->LoadImage("textures/cobblestone.jpg");
	floorTexture = new Texture();
	floorTexture->LoadImage("textures/floor.jpg");
	doorTexture = new Texture();
	doorTexture->LoadImage("textures/green.jpg");
}
void SceneTest::BuildHealthUI() {
	healthUITexture = new Texture();
	healthUITexture->LoadImage("textures/red.jpg");
}

void SceneTest::Update(const float deltaTime) {
	if (speedItem->getActive() == false) { //test for right now - if all enemy dead = roomCleared = true;
		roomCleared = true;
	}
	//enemy and item updates
	if (roomUpdate == false) {
		enemy1->Update(deltaTime);
		if (enemy1->DamageCheck(character) && character->getInvincibility() == false && character->getAttacking() == false) {
			character->setinvincibilityTimer(100); //setting the timer for the invinciblity
			health -= 10; //set characters new health after taking damage
		}

		if (enemy1->WeaponColCheck(character) && character->getAttacking() == true) {
			//Enemy takes damage
			printf("\nEnemy has taken damage");
		}
		if (speedItem->getActive()) {
			speedItem->collisionCheck(character);
		}
		if (healingItem->getActive() && healingItem->collisionCheck(character)) {
			health = health + 20;
			if (health > 50) {
				health = 50;
			}
		}
		if (shovel->getActive()) {
			shovel->collisionCheck(character);
		}
		if (axe->getActive()) {
			axe->collisionCheck(character);
		}
		if (sword->getActive()) {
			sword->collisionCheck(character);
		}
		if (zwei->getActive()) {
			zwei->collisionCheck(character);
		}

		healthBar->setModelMatrix(MMath::translate(Vec3(0.0f, -3.5f, -5.0f)) * MMath::scale(0.05f * (health + 0.01), 0.3f, 0.01f) * MMath::rotate(-10.0f, 1.0, 0.0, 0.0)); //Should make the healthbar smaller when character is damaged by enemy
		enemy1->setModelMatrix(MMath::translate(enemy1->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f));
		//snakeEnemy->setModelMatrix(MMath::translate(snakeEnemy->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f));
		//snakeEnemy->PatrolArea();
	}
	//door and character updates
	character->checkInvincibility(); //checking if the character is invincible
	character->setModelMatrix(MMath::translate(character->getPos()));
	
	character->setModelMatrix(MMath::translate(character->getPos()) * MMath::rotate(character->getRotation(), Vec3(0.0f, 0.0f, 1.0f)));

	//printf("current pos: %f %f %f\n", character->getPos().x, character->getPos().y, character->getPos().z);
}

void SceneTest::Render() const {
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
		enemy1->Render();
		//snakeEnemy->Render();
		if (speedItem->getActive()) {
			//speedItem->Render();
		}
		if (healingItem->getActive()) {
			healingItem->Render();
		}
		if (shovel->getActive()) {
			shovel->Render();
		}
		if (axe->getActive()) {
			axe->Render();
		}
		if (sword->getActive()) {
			sword->Render();
		}
		if (zwei->getActive()) {
			zwei->Render();
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
	glUseProgram(0);
}

bool SceneTest::setSceneCleared() {
	return roomCleared;
}

void SceneTest::getSceneCleared(const bool storedRoom_) {
	roomCleared = storedRoom_;
}

float SceneTest::setCharacterHealth() {
	return health;
}

void SceneTest::getCharacterHealth(const float storedHealth_) {
	health = storedHealth_;
}

Vec3 SceneTest::setCharacterPos() {
	return character->getPos();
}

void SceneTest::getCharacterPos(const Vec3 storedPos_) {
	if (storedPos_.x >= 0 && storedPos_.y >= -1.0 && storedPos_.y <= 1.0) { //entering from left door
		returnedPos = Vec3((storedPos_.x * -1 + 1), storedPos_.y, storedPos_.z);
	}
	else if (storedPos_.x < 0 && storedPos_.y >= -1.0 && storedPos_.y <= 1.0) { //entering from right door
		returnedPos = Vec3((storedPos_.x * -1 - 1), storedPos_.y, storedPos_.z);
	}
	else if (storedPos_.y >= 0 && storedPos_.x >= -1.0 && storedPos_.x <= 1.0) { //entering from top door
		returnedPos = Vec3(storedPos_.x, (storedPos_.y - 1), storedPos_.z);
	}
	else if (storedPos_.y < 0 && storedPos_.x >= -1.0 && storedPos_.x <= 1.0) { //entering from bottom door
		returnedPos = Vec3(storedPos_.x, (storedPos_.y + 1), storedPos_.z);
	}
}

float SceneTest::setCharacterSpeed() {
	return character->getSpeed();
}

void SceneTest::getCharacterSpeed(const float storedSpeed_) {
	speed = storedSpeed_;
}

int SceneTest::setWeapon() {
	return weapon;
}

void SceneTest::getWeapon(const int storedWeapon_) {
	weapon = storedWeapon_;
}

void SceneTest::OnDestroy() {
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (snakeMeshPtr) delete snakeMeshPtr, snakeMeshPtr = nullptr;
	if (snakeTexture) delete snakeTexture, snakeTexture = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (character) delete character, character = nullptr;
	if (healthBar) delete healthBar, healthBar = nullptr;
	if (enemy1) delete enemy1, enemy1 = nullptr;
	if (wall1) delete wall1, wall1 = nullptr;
	if (wall2) delete wall2, wall2 = nullptr;
	if (wall3) delete wall3, wall3 = nullptr;
	if (wall4) delete wall4, wall4 = nullptr;
	if (floor) delete floor, floor = nullptr;
}