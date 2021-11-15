#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene00.h"
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

Scene00::Scene00() : character(nullptr), characterMesh(nullptr), shaderPtr(nullptr), texturePtr(nullptr), boxMesh(nullptr), doorLeft(nullptr), doorRight(nullptr), doorTop(nullptr), doorTexture(nullptr), floor(nullptr), floorTexture(nullptr),
health(NULL), healthBar(nullptr), healthUITexture(nullptr), wall1(nullptr), wall2(nullptr), wall4(nullptr), wallTexture(nullptr), speed(NULL), weapon(NULL) {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene00::~Scene00() {}

bool Scene00::OnCreate() {
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
	wall4->setModelMatrix(MMath::translate(Vec3(0.0, 5.75, -15.0)) * MMath::scale(11.5f, 0.75f, 1.0f));
	floor->setModelMatrix(MMath::translate(Vec3(0.0, 0.0, -17.0)) * MMath::scale(12.4f, 7.5f, 1.0f));
	doorLeft->setModelMatrix(MMath::translate(doorLeft->getPos()) * MMath::scale(1.0f, 1.0f, 1.0f));
	doorTop->setModelMatrix(MMath::translate(doorTop->getPos()) * MMath::scale(1.0f, 1.0f, 1.0f));
	doorRight->setModelMatrix(MMath::translate(doorRight->getPos()) * MMath::scale(1.0f, 1.0f, 1.0f));
	return true;
}

void Scene00::HandleEvents(const SDL_Event& sdlEvent) {
	character->HandleEvents(sdlEvent);
}

int Scene00::SetScene() {
	return sceneNumber;
}

void Scene00::BuildCharacter() {
	ObjLoader::loadOBJ("meshes/Sphere.obj");
	characterMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	texturePtr = new Texture();
	texturePtr->LoadImage("textures/white.png");
	character = new Character(characterMesh, shaderPtr, texturePtr, room);
	character->setSpeed(speed);
	character->setPos(returnedPos); //using for setting the position 
}

void Scene00::BuildAllEnemies() {

}

void Scene00::BuildRoom() {
	ObjLoader::loadOBJ("meshes/Cube.obj");
	boxMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	wallTexture = new Texture();
	wallTexture->LoadImage("textures/cobblestone.jpg");
	floorTexture = new Texture();
	floorTexture->LoadImage("textures/introFloor.jpg");
	doorTexture = new Texture();
	doorTexture->LoadImage("textures/green.jpg");

	wall1 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall2 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall4 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	floor = new StaticMesh(boxMesh, shaderPtr, floorTexture);
	doorRight = new Door(boxMesh, shaderPtr, doorTexture, Vec3(9.5, 0.0, -15.0f));
	doorLeft = new Door(boxMesh, shaderPtr, doorTexture, Vec3(-9.5, 0.0, -15.0f));
	doorTop = new Door(boxMesh, shaderPtr, doorTexture, Vec3(0.0, 4.5, -15.0f));
}
void Scene00::BuildHealthUI() {
	healthUITexture = new Texture();
	healthUITexture->LoadImage("textures/red.jpg");
	healthBar = new HealthUI(boxMesh, shaderPtr, healthUITexture);
}

void Scene00::Update(const float deltaTime) {
	//enemy and item updates
	if (roomUpdate == false) {
		
	}
	//door and character updates
	if (doorTop->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
		sceneNumber = 2;
	}
	if (doorRight->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
		sceneNumber = 1;
	}
	if (doorLeft->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
		sceneNumber = 1;
	}
	character->checkInvincibility(); //checking if the character is invincible
	character->setModelMatrix(MMath::translate(character->getPos()));
	//printf("current pos: %f %f %f\n", character->getPos().x, character->getPos().y, character->getPos().z);
}

void Scene00::Render() const {
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
		
	}
	//door and character renders
	if (character->getVisibility()) {
		character->Render();
	}
	//Room and UI renders
	healthBar->Render();
	wall1->Render();
	wall2->Render();
	wall4->Render();
	floor->Render();
	doorLeft->Render();
	doorTop->Render();
	doorRight->Render();
	glUseProgram(0);
}

bool Scene00::setSceneCleared() {
	return roomCleared;
}

void Scene00::getSceneCleared(const bool storedRoom_) {
	roomCleared = storedRoom_;
}

float Scene00::setCharacterHealth() {
	return health;
}

void Scene00::getCharacterHealth(const float storedHealth_) {
	health = storedHealth_;
}

Vec3 Scene00::setCharacterPos() {
	return character->getPos();
}

void Scene00::getCharacterPos(const Vec3 storedPos_) {
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
}

float Scene00::setCharacterSpeed() {
	return character->getSpeed();
}

void Scene00::getCharacterSpeed(const float storedSpeed_) {
	speed = storedSpeed_;
}

int Scene00::setWeapon() {
	return weapon;
}

void Scene00::getWeapon(const int storedWeapon_) {
	weapon = storedWeapon_;
}

void Scene00::OnDestroy() {
	if (characterMesh) delete characterMesh, characterMesh = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (character) delete character, character = nullptr;
	if (healthBar) delete healthBar, healthBar = nullptr;
	if (wall1) delete wall1, wall1 = nullptr;
	if (wall2) delete wall2, wall2 = nullptr;
	if (wall4) delete wall4, wall4 = nullptr;
	if (floor) delete floor, floor = nullptr;
	if (doorLeft) delete doorLeft, doorLeft = nullptr;
	if (doorRight) delete doorLeft, doorLeft = nullptr;
}