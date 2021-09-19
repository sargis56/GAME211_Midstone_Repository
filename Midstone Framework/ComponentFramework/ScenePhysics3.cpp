#include <glew.h>
#include <iostream>
//#include <tgmath.h>
#include "Debug.h"
#include "ScenePhysics3.h"
#include "DemoObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Debug.h"
#include "Physics.h"
using namespace std;

ScenePhysics3::ScenePhysics3() : demoObject(nullptr), demoObject2(nullptr), meshPtr(nullptr), shaderPtr(nullptr), texturePtr(nullptr), cubeMeshPtr(nullptr){
	Debug::Info("Created ScenePhysics3: ", __FILE__, __LINE__);
}

ScenePhysics3::~ScenePhysics3() {}

bool ScenePhysics3::OnCreate() {
	//Setting up the scene lights
	numLight = 2;
	lightArray[0] = Vec3(0.0f, 40.0f, 0.0f);
	lightArray[1] = Vec3(0.0f, -20.0f, 0.0f);
	Light1 = new Light(Vec3(0.0f, 40.0f, 0.0f), Vec3(0.6f, 0.6f, 0.6f), Vec3(0.1f, 0.6f, 0.6f));
	
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
	demoObject2 = new DemoObject(cubeMeshPtr, shaderPtr, texturePtr);
	if (demoObject2 == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	//Plane set up
	planeMain.set(-1.0f, 1.0f, 0.0f, 0.0f);
	//Setting up the balls
	demoObject->setPos(Vec3(-2.0f, 2.0f, 0.0f));
	demoObject2->setPos(Vec3(1.2f, -1.6f, 0.0f));
	demoObject->setRadius(1.12f);
	demoObject->setVel(Vec3(1.0f, 0.0f, 0.0f));
	return true;
}

void ScenePhysics3::HandleEvents(const SDL_Event& sdlEvent) {

}
void ScenePhysics3::Update(const float deltaTime) {
	//Updating object every frame
	demoObject->Update(deltaTime);
	//Simple Motion
	Physics::SimpleNewtonMotion(*demoObject, deltaTime);
	//Sphere Sphere Collision
	Physics::PlaneSphereCollisionResponse(*demoObject, planeMain);
	cout << "test print: (" << demoObject->getVel().x << ", " << demoObject->getVel().y << ", " << demoObject->getVel().z << ")" << endl;
	//Updating the models
	demoObject->setModelMatrix(MMath::translate(demoObject->getPos()) * MMath::scale(demoObject->getRadius(), demoObject->getRadius(), demoObject->getRadius()));
	demoObject2->setModelMatrix(MMath::translate(demoObject2->getPos())* MMath::rotate(45.0f, Vec3(0.0f,0.0f,1.0f)) * MMath::scale(6.0, 0.1f, 1.0f));
}
void ScenePhysics3::Render() const {
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

	glUniform3fv(demoObject->getShader()->getUniformID("lightPos[0]"), 2, *lightArray);
	glUniform1f(demoObject->getShader()->getUniformID("numLight"), numLight);
	//Rendering the balls
	demoObject->Render();
	demoObject2->Render();
	glUseProgram(0);
}


void ScenePhysics3::OnDestroy() {
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (demoObject) delete demoObject, demoObject = nullptr;
	if (demoObject2) delete demoObject2, demoObject2 = nullptr;
}