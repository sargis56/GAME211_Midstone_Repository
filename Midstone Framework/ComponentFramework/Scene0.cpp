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

Scene0::Scene0() : character(nullptr), meshPtr(nullptr), shaderPtr(nullptr), texturePtr(nullptr) {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene0::~Scene0() {}

bool Scene0::OnCreate() {
	room.setRoomBorders(Vec3(-9.0, -4.0, 0.0), Vec3(9.0, 4.0, 0.0));
	numLight = 2;
	lightArray[0] = Vec3(0.0f, 40.0f, 0.0f);
	lightArray[1] = Vec3(0.0f, -20.0f, 0.0f);

	projectionMatrix = MMath::perspective(30.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	ObjLoader::loadOBJ("meshes/Sphere.obj");

	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	shaderPtr = new Shader("shaders/multiPhongVert.glsl", "shaders/multiPhongFrag.glsl");

	texturePtr = new Texture();

	texturePtr->LoadImage("textures/skull_texture.jpg");

	character = new Character(meshPtr, shaderPtr, texturePtr, room);
	if (character == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	enemy1 = new RatEnemy(meshPtr, shaderPtr, texturePtr, room);
	enemy1->setPos(Vec3(0.0, 0.0, -15.0));
	character->setPos(Vec3(0.0, 0.0, -15.0));

	speed = 50;
	return true;
}

void Scene0::HandleEvents(const SDL_Event& sdlEvent) {
	character->HandleEvents(sdlEvent);
}

float Scene0::setSpeed()
{
	return speed;
}

void Scene0::getSpeed(const float speed_) {
	speed = speed_;
}

void Scene0::Update(const float deltaTime) {
	character->Update(deltaTime);
	enemy1->Update(deltaTime);
	//printf("%f\n", speed);
	character->setModelMatrix(MMath::translate(character->getPos()));
	enemy1->setModelMatrix(MMath::translate(enemy1->getPos()));
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

	//Multi Shader
	glUniform3fv(character->getShader()->getUniformID("lightPos[0]"), 2, *lightArray);
	glUniform1f(character->getShader()->getUniformID("numLight"), numLight);

	character->Render();
	enemy1->Render();
	glUseProgram(0);
}


void Scene0::OnDestroy() {
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (character) delete character, character = nullptr;
	if (enemy1) delete enemy1, enemy1 = nullptr;
}