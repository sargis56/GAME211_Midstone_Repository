#include <glew.h>
#include <iostream>
//#include <tgmath.h>
#include "Debug.h"
#include "ScenePhysics2.h"
#include "DemoObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Debug.h"
#include "Physics.h"
using namespace std;

ScenePhysics2::ScenePhysics2() : sphereObject(nullptr), sphereObject2(nullptr), meshPtr(nullptr), shaderPtr(nullptr), texturePtr(nullptr) {
	Debug::Info("Created ScenePhysics2: ", __FILE__, __LINE__);
}

ScenePhysics2::~ScenePhysics2() {}

bool ScenePhysics2::OnCreate() {
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

	sphereObject = new DemoObject(meshPtr, shaderPtr, texturePtr);
	if (sphereObject == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	sphereObject2 = new DemoObject(meshPtr, shaderPtr, texturePtr);
	if (sphereObject2 == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	//Setting up the balls
	sphereObject->setPos(Vec3(3.0f, 4.0f, 0.0f));
	sphereObject2->setPos(Vec3(2.0f, 2.0f, 0.0f));
	sphereObject->setRadius(1.12f);
	sphereObject2->setRadius(1.12f);
	sphereObject->setVel(Vec3(0.0f, -4.0f, 0.0f));
	sphereObject2->setVel(Vec3(0.0f, 0.0f, 0.0f));
	sphereObject->setMass(1.0f);
	sphereObject2->setMass(1.0f);
	//sphereObject2->setVel(Vec3(0.0f, 1.0f, 0.0f));
	return true;
}

void ScenePhysics2::HandleEvents(const SDL_Event& sdlEvent) {

}
void ScenePhysics2::Update(const float deltaTime) {
	//Updating object every frame
	sphereObject->Update(deltaTime);
	sphereObject2->Update(deltaTime);
	//Simple Motion
	Physics::SimpleNewtonMotion(*sphereObject, deltaTime);
	Physics::SimpleNewtonMotion(*sphereObject2, deltaTime);
	//Sphere Sphere Collision
	Physics::SphereSphereCollisionResponse(*sphereObject, *sphereObject2, 0.8f);
	cout << "test print: (" << sphereObject->getVel().x << ", " << sphereObject->getVel().y << ", " << sphereObject->getVel().z << ")" << endl;
	//Updating the models
	sphereObject->setModelMatrix(MMath::translate(sphereObject->getPos()) * MMath::scale(sphereObject->getRadius(), sphereObject->getRadius(), sphereObject->getRadius()));
	sphereObject2->setModelMatrix(MMath::translate(sphereObject2->getPos()) * MMath::scale(sphereObject2->getRadius(), sphereObject2->getRadius(), sphereObject2->getRadius()));
	//if (Physics::SphereSphereCollision(*sphereObject, *sphereObject2)) {
	//	printf("e\n");
	//}
}
void ScenePhysics2::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	/// Draw your scene here
	GLuint program = sphereObject->getShader()->getProgram();
	glUseProgram(program);

	/// These pass the matricies and the light position to the GPU
	glUniformMatrix4fv(sphereObject->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(sphereObject->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);

	glUniform3fv(sphereObject->getShader()->getUniformID("lightPos[0]"), 2, *lightArray);
	glUniform1f(sphereObject->getShader()->getUniformID("numLight"), numLight);
	//Rendering the balls
	sphereObject->Render();
	sphereObject2->Render();
	glUseProgram(0);
}


void ScenePhysics2::OnDestroy() {
	if (meshPtr) delete meshPtr, meshPtr = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (sphereObject) delete sphereObject, sphereObject = nullptr;
	if (sphereObject2) delete sphereObject2, sphereObject2 = nullptr;
}