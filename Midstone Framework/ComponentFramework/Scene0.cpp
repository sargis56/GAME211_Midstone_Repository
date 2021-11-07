#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene0.h"
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

Scene0::Scene0() : character(nullptr), meshPtr(nullptr), shaderPtr(nullptr), texturePtr(nullptr), boxMesh(nullptr), doorLeft(nullptr), doorTexture(nullptr), enemy1(nullptr), floor(nullptr), floorTexture(nullptr),
health(NULL), healthBar(nullptr), healthUITexture(nullptr), ratMeshPtr(nullptr), turretTexture(nullptr), wall1(nullptr), wall2(nullptr), wall3(nullptr), wall4(nullptr), wallTexture(nullptr) {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene0::~Scene0() {}

bool Scene0::OnCreate() {
	room.setRoomBorders(Vec3(-9.0, -4.0, 0.0), Vec3(9.0, 4.0, 0.0));
	light1 = Vec3(0.0f, 40.0f, 0.0f);

	projectionMatrix = MMath::perspective(30.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	BuildCharacter();
	character = new Character(meshPtr, shaderPtr, texturePtr, room);
	if (character == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	character->setPos(returnedPos);
	//character->setHealth(50);
	BuildRat();
	enemy1 = new TeslaTowerEnemy(ratMeshPtr, shaderPtr, turretTexture, room, character);
	enemy1->OnCreate();
	enemy1->setPos(Vec3(2.0, 2.0, -15.0));
	BuildWall();
	wall1 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall2 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall3 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall4 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	BuildFloor();
	floor = new StaticMesh(boxMesh, shaderPtr, floorTexture);
	BuildDoor();
	//doorLeft = new Door(boxMesh, shaderPtr, doorTexture, Vec3(-9.5, 0.0, -15.0f));
	doorLeft = new Door(boxMesh, shaderPtr, doorTexture, Vec3(0.0f, 4.5f, -15.0f));
	BuildHealthUI();
	healthBar = new HealthUI(boxMesh, shaderPtr, healthUITexture);
	//health = 50;
	speedItem = new SpeedItem(meshPtr, shaderPtr, doorTexture, 0.2f, Vec3(3.0f, 3.0f, -15.0f));

	healthBar->setModelMatrix(MMath::translate(Vec3(0.0f, -3.5f, -5.0f)) * MMath::scale(0.05f * (health + 0.01), 0.3f, 0.01f) * MMath::rotate(-10.0f, 1.0, 0.0, 0.0));
	wall1->setModelMatrix(MMath::translate(Vec3(-11.0, 0.0, -15.0)) * MMath::scale(0.75f, 5.0f, 1.0f));
	wall2->setModelMatrix(MMath::translate(Vec3(11.0, 0.0, -15.0)) * MMath::scale(0.75f, 5.0f, 1.0f));
	wall3->setModelMatrix(MMath::translate(Vec3(0.0, -5.75, -15.0)) * MMath::scale(11.5f, 0.75f, 1.0f));
	wall4->setModelMatrix(MMath::translate(Vec3(0.0, 5.75, -15.0)) * MMath::scale(11.5f, 0.75f, 1.0f));
	floor->setModelMatrix(MMath::translate(Vec3(0.0, 0.0, -17.0)) * MMath::scale(11.4f, 5.5f, 1.0f));
	doorLeft->setModelMatrix(MMath::translate(doorLeft->getPos()) * MMath::scale(1.0f, 1.0f, 1.0f));
	speedItem->setModelMatrix(MMath::translate(speedItem->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f));
	return true;
}

void Scene0::HandleEvents(const SDL_Event& sdlEvent) {
	character->HandleEvents(sdlEvent);
}

int Scene0::SetScene() {
	return sceneNumber;
}

void Scene0::BuildCharacter() {
	ObjLoader::loadOBJ("meshes/Sphere.obj");
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	texturePtr = new Texture();
	texturePtr->LoadImage("textures/skull_texture.jpg");
}

void Scene0::BuildRat() {
	ObjLoader::loadOBJ("meshes/Enemies/MagicTurret.obj");
	ratMeshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	turretTexture = new Texture();
	turretTexture->LoadImage("textures/Enemies/Turret_Texture.jpg");
}

void Scene0::BuildWall() {
	ObjLoader::loadOBJ("meshes/Cube.obj");
	boxMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	wallTexture = new Texture();
	wallTexture->LoadImage("textures/cobblestone.jpg");
}

void Scene0::BuildFloor() {
	floorTexture = new Texture();
	floorTexture->LoadImage("textures/floor.jpg");
}

void Scene0::BuildDoor() {
	doorTexture = new Texture();
	doorTexture->LoadImage("textures/green.jpg");
}
void Scene0::BuildHealthUI() {
	healthUITexture = new Texture();
	healthUITexture->LoadImage("textures/red.jpg");
}

void Scene0::Update(const float deltaTime) {
	character->Update(deltaTime);
	enemy1->Update(deltaTime);

	if (enemy1->DamageCheck(character) && character->getInvincibility() == false) {
		character->setinvincibilityTimer(100);
		health -= 10; //set characters new health after taking damage
		healthBar->setModelMatrix(MMath::translate(Vec3(0.0f, -3.5f, -5.0f)) * MMath::scale(0.05f * (health + 0.01), 0.3f, 0.01f) * MMath::rotate(-10.0f, 1.0, 0.0, 0.0)); //Should make the healthbar smaller when character is damaged by enemy
	}
	if (doorLeft->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
		sceneNumber = 2;
	}
	if (speedItem->getActive()) {
		speedItem->collisionCheck(character);
	}
	character->checkInvincibility();
	character->setModelMatrix(MMath::translate(character->getPos()));
	enemy1->setModelMatrix(MMath::translate(enemy1->getPos()) * MMath::scale(0.5f,0.5f,0.5f));

	//printf("current pos: %f %f %f\n", character->getPos().x, character->getPos().y, character->getPos().z);


}

void Scene0::Render() const {
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
	if (character->getVisibility()) {
		character->Render();
	}
	healthBar->Render();
	enemy1->Render();
	wall1->Render();
	wall2->Render();
	wall3->Render();
	wall4->Render();
	floor->Render();
	doorLeft->Render();
	if (speedItem->getActive()) {
		speedItem->Render();
	}
	glUseProgram(0);
}

float Scene0::setCharacterHealth() {
	return health;
}

void Scene0::getCharacterHealth(const float storedHealth_) {
	health = storedHealth_;
}

Vec3 Scene0::setCharacterPos() {
	return character->getPos();
}

void Scene0::getCharacterPos(const Vec3 storedPos_) {
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

void Scene0::OnDestroy() {
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
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
	if (doorLeft) delete doorLeft, doorLeft = nullptr;
}