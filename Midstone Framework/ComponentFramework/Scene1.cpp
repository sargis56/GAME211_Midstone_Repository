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

Scene1::Scene1() : character(nullptr), meshPtr(nullptr), shaderPtr(nullptr), texturePtr(nullptr) {
	Debug::Info("Created Scene1: ", __FILE__, __LINE__);
}

Scene1::~Scene1() {}

bool Scene1::OnCreate() {
	numLight = 2;
	lightArray[0] = Vec3(0.0f, 40.0f, 0.0f);
	lightArray[1] = Vec3(0.0f, -20.0f, 0.0f);

	projectionMatrix = MMath::perspective(30.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	if (ObjLoader::loadOBJ("meshes/Sphere.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	shaderPtr = new Shader("shaders/multiPhongVert.glsl", "shaders/multiPhongFrag.glsl");

	texturePtr = new Texture();
	if (meshPtr == nullptr || shaderPtr == nullptr || texturePtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}


	if (texturePtr->LoadImage("textures/skull_texture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}

	character = new Character(meshPtr, shaderPtr, texturePtr);
	if (character == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	character->setPos(Vec3(0.0, 0.0, -15.0));
	character->setModelMatrix(MMath::translate(character->getPos()));

	return true;
}

void Scene1::HandleEvents(const SDL_Event& sdlEvent) {
	character->HandleEvents(sdlEvent);
}

float Scene1::setSpeed()
{
	return speed;
}

void Scene1::getSpeed(const float speed_) {
	speed = speed_;
}

void Scene1::Update(const float deltaTime) {
	character->Update(deltaTime);
	printf("%f\n", speed);
	static float rotation = 0.0f;
	rotation += 1.5f;
	character->setModelMatrix(MMath::translate(character->getPos()));
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

	//Multi Shader

	glUniform3fv(character->getShader()->getUniformID("lightPos[0]"), 2, *lightArray);
	glUniform1f(character->getShader()->getUniformID("numLight"), numLight);

	character->Render();

	glUseProgram(0);
}


void Scene1::OnDestroy() {
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (character) delete character, character = nullptr;
}