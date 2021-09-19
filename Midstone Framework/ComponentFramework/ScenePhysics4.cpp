#include <glew.h>
#include <iostream>
//#include <tgmath.h>
#include "Debug.h"
#include "ScenePhysics4.h"
#include "demoObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Debug.h"
#include "Physics.h"
using namespace std;

ScenePhysics4::ScenePhysics4() : logObject(nullptr), waterObject(nullptr), meshPtr(nullptr), shaderPtr(nullptr), textureLogPtr(nullptr), textureWaterPtr(nullptr), cubeMeshPtr(nullptr){
	Debug::Info("Created ScenePhysics4: ", __FILE__, __LINE__);
}

ScenePhysics4::~ScenePhysics4() {
}

bool ScenePhysics4::OnCreate() {
	//Setting up the scene lights
	lightPos = Vec3(8.0f, 0.0f, 0.0f);
	
	projectionMatrix = MMath::perspective(60.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	if (ObjLoader::loadOBJ("meshes/Sphere.obj") == false) {
		return false;
	}
	meshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	if (ObjLoader::loadOBJ("meshes/Cube.obj") == false) {
		return false;
	}
	cubeMeshPtr = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	textureLogPtr = new Texture();
	textureWaterPtr = new Texture();
	if (meshPtr == nullptr || shaderPtr == nullptr || textureLogPtr == nullptr || textureWaterPtr == (nullptr) || cubeMeshPtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}
	if (textureLogPtr->LoadImage("textures/log.png") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (textureWaterPtr->LoadImage("textures/waterTexture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	logObject = new DemoObject(meshPtr, shaderPtr, textureLogPtr);
	if (logObject == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	waterObject = new DemoObject(cubeMeshPtr, shaderPtr, textureWaterPtr);
	if (waterObject == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	//Setting up the objects
	logObject->setPos(Vec3(0.0f, 0.0f, 0.0f)); //3.0f
	waterObject->setPos(Vec3(0.0f, 0.0f, 0.0f));
	logObject->setRadius(1.0f);
	logObject->setMass(1.0f); //1000.0f
	return true;
}

void ScenePhysics4::HandleEvents(const SDL_Event& sdlEvent) {
}
void ScenePhysics4::Update(const float deltaTime) {
	//Updating object every frame
	//Buoyancy Motion
	Physics::BuoyancyApplyForces(*logObject, waterObject->getPos().y, 0.9f, 5.0f, 1.0f);
	//Simple Motion
	Physics::VerletMotion(*logObject, deltaTime, previousAccel); //Verlet
	previousAccel = logObject->getAccel(); //Updating previousAccel
	//Physics::SimpleNewtonMotion(*logObject, deltaTime);        //Newton
	//Updating the models
	logObject->Update(deltaTime);
	//logObject->setModelMatrix(MMath::translate(logObject->getPos()) * MMath::scale(logObject->getRadius() * 0.07, logObject->getRadius() * 0.07, logObject->getRadius() * 0.07));
	logObject->setModelMatrix(MMath::translate(logObject->getPos()) * MMath::scale(logObject->getRadius(), logObject->getRadius(), logObject->getRadius()));
	waterObject->setModelMatrix(MMath::translate(waterObject->getPos()) * MMath::scale(6.0, 0.05f, 6.0f));
	//Printing information
	cout << "pos print: (" << logObject->getPos().x << ", " << logObject->getPos().y << ", " << logObject->getPos().z << ")" << endl;
	//cout << "vel print: (" << logObject->getVel().x << ", " << logObject->getVel().y << ", " << logObject->getVel().z << ")" << endl;
}
void ScenePhysics4::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	/// Draw your scene here
	GLuint program = logObject->getShader()->getProgram();
	glUseProgram(program);

	/// These pass the matricies and the light position to the GPU
	glUniformMatrix4fv(logObject->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(logObject->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);

	glUniform3fv(logObject->getShader()->getUniformID("lightPos"), 1, lightPos);
	//Rendering the objects
	logObject->Render();
	waterObject->Render();
	glUseProgram(0);
}


void ScenePhysics4::OnDestroy() {
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (textureLogPtr) delete textureLogPtr, textureLogPtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (logObject) delete logObject, logObject = nullptr;
	if (waterObject) delete waterObject, waterObject = nullptr;;
}