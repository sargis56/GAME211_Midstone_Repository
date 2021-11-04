#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene1.h"
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

Scene1::Scene1() : character(nullptr), meshPtr(nullptr), shaderPtr(nullptr), texturePtr(nullptr), boxMesh(nullptr), doorRight(nullptr), doorTexture(nullptr), enemy1(nullptr), floor(nullptr), floorTexture(nullptr),
health(NULL), /*healthBar(nullptr),*/ healthUITexture(nullptr), ratMeshPtr(nullptr), enemyDemon0(nullptr), demonMeshPtr(nullptr), demonTexture(nullptr), ratTexture(nullptr), wall1(nullptr), wall2(nullptr), 
wall3(nullptr), wall4(nullptr), wallTexture(nullptr) {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene1::~Scene1() {}

bool Scene1::OnCreate() {
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
	BuildSnake();
	enemy1 = new SnakeEnemy(ratMeshPtr, shaderPtr, ratTexture, room);
	enemy1->setPos(Vec3(5.0, 4.0, -15.0));
	enemy1->OnCreate();
	//enemy1->setPos(Vec3(5.0, 4.0, -15.0));

	BuildDemon();
	enemyDemon0 = new DemonEnemy(demonMeshPtr, shaderPtr, demonTexture, room, 5);
	enemyDemon0->OnCreate();
	enemyDemon0->setPos(Vec3(5.0, -4.0, -15.0));

	BuildWall();
	wall1 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall2 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall3 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	wall4 = new StaticMesh(boxMesh, shaderPtr, wallTexture);
	BuildFloor();
	floor = new StaticMesh(boxMesh, shaderPtr, floorTexture);
	BuildDoor();
	doorRight = new Door(boxMesh, shaderPtr, doorTexture, Vec3(9.5, 0.0, -15));
	BuildHealthUI();
	//healthBar = new HealthUI(boxMesh, shaderPtr, healthUITexture);

	//healthBar->setModelMatrix(MMath::translate(Vec3(0.0f, -3.5f, -5.0f)) * MMath::scale(0.05f * (health + 0.01), 0.3f, 0.01f) * MMath::rotate(-10.0f, 1.0, 0.0, 0.0));
	wall1->setModelMatrix(MMath::translate(Vec3(-11.0, 0.0, -15.0)) * MMath::scale(0.75f, 5.0f, 1.0f));
	wall2->setModelMatrix(MMath::translate(Vec3(11.0, 0.0, -15.0)) * MMath::scale(0.75f, 5.0f, 1.0f));
	wall3->setModelMatrix(MMath::translate(Vec3(0.0, -5.75, -15.0)) * MMath::scale(11.5f, 0.75f, 1.0f));
	wall4->setModelMatrix(MMath::translate(Vec3(0.0, 5.75, -15.0)) * MMath::scale(11.5f, 0.75f, 1.0f));
	floor->setModelMatrix(MMath::translate(Vec3(0.0, 0.0, -17.0)) * MMath::scale(11.4f, 5.5f, 1.0f));
	doorRight->setModelMatrix(MMath::translate(doorRight->getPos()) * MMath::scale(1.0f, 1.0f, 1.0f));
	//health = 50;
	return true;
}

void Scene1::HandleEvents(const SDL_Event& sdlEvent) {
	character->HandleEvents(sdlEvent);
}

int Scene1::SetScene() {
	return sceneNumber;
}

void Scene1::BuildCharacter() {
	ObjLoader::loadOBJ("meshes/Sphere.obj");
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	texturePtr = new Texture();
	texturePtr->LoadImage("textures/skull_texture.jpg");
}

void Scene1::BuildSnake() { 
	ObjLoader::loadOBJ("meshes/Enemies/Rat.obj");
	ratMeshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	ratTexture = new Texture();
	ratTexture->LoadImage("textures/Enemies/Rat_Texture.jpg");
}

void Scene1::BuildDemon() {
	ObjLoader::loadOBJ("meshes/Enemies/Demon.obj");
	demonMeshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	demonTexture = new Texture();
	demonTexture->LoadImage("textures/Enemies/Demon_Texture.jpg");
}

void Scene1::BuildWall() {
	ObjLoader::loadOBJ("meshes/Cube.obj");
	boxMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	wallTexture = new Texture();
	wallTexture->LoadImage("textures/cobblestone.jpg");
}

void Scene1::BuildFloor() {
	floorTexture = new Texture();
	floorTexture->LoadImage("textures/floor.jpg");
}
void Scene1::BuildDoor() {
	doorTexture = new Texture();
	doorTexture->LoadImage("textures/green.jpg");
}

void Scene1::BuildHealthUI() {
	healthUITexture = new Texture();
	healthUITexture->LoadImage("textures/red.jpg");
}

void Scene1::Update(const float deltaTime) {
	character->Update(deltaTime);
	enemy1->Update(deltaTime);
	enemyDemon0->Update(deltaTime);

	float enemyRot = enemy1->FollowPlayer(character);
	float enemyRot1 = enemyDemon0->FollowPlayer(character);

	//if (enemy1->DamageCheck(character)) {
	//	printf("DMG\n");
	//}
	//if (enemyDemon0->DamageCheck(character)) {
	//	printf("DMG\n");
	//}
	if (enemy1->DamageCheck(character) && enemyDemon0->DamageCheck(character)) {
		//LOG(charHealth);
		health -= 10; //set characters new health after taking damage
		//LOG(charHealth);
		//healthBar->setModelMatrix(MMath::translate(Vec3(0.0f, -3.5f, -5.0f)) * MMath::scale(0.05f * (health + 0.01), 0.3f, 0.01f) * MMath::rotate(-10.0f, 1.0, 0.0, 0.0)); //Should make the healthbar smaller when character is damaged by enemy
	}
	if (doorRight->CollisionCheck(character)) {  //If character touches the door, switch scene to next level
		sceneNumber = 1;
	}

	//printf("%f\n", health);
	character->setModelMatrix(MMath::translate(character->getPos()));

	enemy1->setModelMatrix(MMath::translate(enemy1->getPos()) * MMath::rotate(enemyRot, Vec3(0.0f, 0.0f, 1.0f)) * MMath::rotate(90.0f, Vec3(0.0f, 0.0f, 1.0f)) * MMath::scale(0.5f, 0.5f, 0.5f));
	enemyDemon0->setModelMatrix(MMath::translate(enemyDemon0->getPos()) * MMath::rotate(enemyRot1, Vec3(0.0f, 0.0f, 1.0f)) * MMath::rotate(90.0f, Vec3(0.0f, 0.0f, 1.0f)) * MMath::scale(0.5f, 0.5f, 0.5f));

	//printf("current pos: %f %f %f\n", character->getPos().x, character->getPos().y, character->getPos().z);


}

void Scene1::Render() const {
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

	character->Render();
	enemy1->Render();
	enemyDemon0->Render();
	wall1->Render();
	wall2->Render();
	wall3->Render();
	wall4->Render();
	floor->Render();
	//healthBar->Render();
	doorRight->Render();
	glUseProgram(0);
}

float Scene1::setCharacterHealth() {
	return health;
}

void Scene1::getCharacterHealth(const float stats_) {
	health = stats_;
}

Vec3 Scene1::setCharacterPos() {
	return character->getPos();
}

void Scene1::getCharacterPos(const Vec3 storedPos_) {
	if (storedPos_.x >= 0 && storedPos_.y >= -1.0 && storedPos_.y <= 1.0) {
		returnedPos = Vec3((storedPos_.x * -1 + 1), storedPos_.y, storedPos_.z);
	}
	else if (storedPos_.x < 0 && storedPos_.y >= -1.0 && storedPos_.y <= 1.0) {
		returnedPos = Vec3((storedPos_.x * -1 - 1), storedPos_.y, storedPos_.z);
	}
	else if (storedPos_.y >= 0 && storedPos_.x >= -1.0 && storedPos_.x <= 1.0) {
		returnedPos = Vec3(storedPos_.x, (storedPos_.y - 1), storedPos_.z);
	}
	else if (storedPos_.y < 0 && storedPos_.x >= -1.0 && storedPos_.x <= 1.0) {
		returnedPos = Vec3(storedPos_.x, (storedPos_.y + 1), storedPos_.z);
	}
}


void Scene1::OnDestroy() {
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (character) delete character, character = nullptr;
	if (enemy1) delete enemy1, enemy1 = nullptr;
	if (enemyDemon0) delete enemyDemon0, enemyDemon0 = nullptr;
	if (wall1) delete wall1, wall1 = nullptr;
	if (wall2) delete wall2, wall2 = nullptr;
	if (wall3) delete wall3, wall3 = nullptr;
	if (wall4) delete wall4, wall4 = nullptr;
	if (floor) delete floor, floor = nullptr;
	if (doorRight) delete doorRight, doorRight = nullptr;
	//if (healthBar) delete healthBar, healthBar = nullptr;
}