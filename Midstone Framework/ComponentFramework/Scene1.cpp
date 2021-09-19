#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene1.h"
#include "DemoObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Debug.h"
#include "Physics.h"
using namespace std;

Scene1::Scene1() : demoObject(nullptr), meshPtr(nullptr), shaderPtr(nullptr), texturePtr(nullptr) {
	Debug::Info("Created Scene1: ", __FILE__, __LINE__);
}

Scene1::~Scene1() {}

bool Scene1::OnCreate() {
	numLight = 2;
	lightArray[0] = Vec3(0.0f, 40.0f, 0.0f);
	lightArray[1] = Vec3(0.0f, -20.0f, 0.0f);
	Light1 = new Light(Vec3(0.0f, 40.0f, 0.0f), Vec3(0.6f, 0.6f, 0.6f), Vec3(0.1f, 0.6f, 0.6f));
	
	//cout << "position: (" << Light1->position.x << ", " << Light1->position.y << ", " << Light1->position.z << ")" << endl;
	//cout << "diffuse: (" << Light1->diffuse.x << ", " << Light1->diffuse.y << ", " << Light1->diffuse.z << ")" << endl;
	//cout << "specular: (" << Light1->specular.x << ", " << Light1->specular.y << ", " << Light1->specular.z << ")" << endl;
//	position = Vec3(0.0f, 40.0f, 0.0f);
//	diffuse = Vec3(0.6f, 0.6f, 0.6f);
//	specular = Vec3(0.1f, 0.6f, 0.6f);

	projectionMatrix = MMath::perspective(30.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	if (ObjLoader::loadOBJ("meshes/Sub.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	//Struct Shader
	//shaderPtr = new Shader("shaders/phongVertStructs.glsl", "shaders/phongFragStructs.glsl");

	//Multi Shader
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

	demoObject = new DemoObject(meshPtr, shaderPtr, texturePtr);
	if (demoObject == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	demoObject->setPos(Vec3(-5.0, 0.0, 0.0));
	demoObject->setModelMatrix(MMath::translate(demoObject->getPos()));

	return true;
}

void Scene1::HandleEvents(const SDL_Event& sdlEvent) {

}

void Scene1::Update(const float deltaTime) {
	demoObject->Update(deltaTime);

	static float rotation = 0.0f;
	rotation += 1.5f;
	//Matrix4 modelM = (MMath::rotate(180.0f, Vec3(1.0f, 0.0f, 0.0f)));
	Matrix4 modelM = MMath::rotate(rotation, Vec3(0.0f, 1.0f, 0.0f));
	demoObject->setModelMatrix(modelM);

	/*demoObject->setVel(Vec3(1.0, 0.0, 0.0));
	Physics::SimpleNewtonMotion(*demoObject, deltaTime);
	demoObject->setModelMatrix(MMath::translate(demoObject->getPos()));*/
}

void Scene1::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	/// Draw your scene here
	GLuint program = demoObject->getShader()->getProgram();
	glUseProgram(program);

	/// These pass the matricies and the light position to the GPU
	glUniformMatrix4fv(demoObject->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(demoObject->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);

	//Struct Shader
	/*
	glUniform3fv(demoObject->getShader()->getUniformID("position"), 1, Light1->position);
	glUniform3fv(demoObject->getShader()->getUniformID("diffuse"), 1, Light1->diffuse);
	glUniform3fv(demoObject->getShader()->getUniformID("specular"), 1, Light1->specular);
	*/
	//Multi Shader
	
	glUniform3fv(demoObject->getShader()->getUniformID("lightPos[0]"), 2, *lightArray);
	glUniform1f(demoObject->getShader()->getUniformID("numLight"), numLight);
	
	demoObject->Render();

	glUseProgram(0);
}


void Scene1::OnDestroy() {
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (demoObject) delete demoObject, demoObject = nullptr;
}