#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene2.h"
#include "DemoObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
//#include "Debug.h"
#include "Physics.h"
#include "Timer.h"
#include <SDL_events.h>

Scene2::Scene2(): skull(nullptr), leftEye(nullptr), rightEye(nullptr), meshPtrSkull(nullptr), meshPtrEye(nullptr), meshPtrCrown(nullptr), shaderPtr(nullptr), skullTexturePtr(nullptr), eyeTexturePtr(nullptr), crownTexturePtr(nullptr), trackball(nullptr) {
	trackball = new Trackball();
	Debug::Info("Created Scene2: ", __FILE__, __LINE__);
}

Scene2::~Scene2() {
	delete trackball;
}

bool Scene2::OnCreate() {
	lightPos = Vec3(8.0f, 0.0f, 0.0f);
	projectionMatrix = MMath::perspective(40.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	if (skullCreate() == false) {
		return false;
	}

	//Eyes
	if (ObjLoader::loadOBJ("meshes/Sphere.obj") == false) {
		return false;
	}
	meshPtrEye = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	//Crown
	if (ObjLoader::loadOBJ("meshes/WrenlyPrinceCrown.obj") == false) {
		return false;
	}
	meshPtrCrown = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	eyeTexturePtr = new Texture();
	if (eyeTexturePtr->LoadImage("textures/evileye.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	crownTexturePtr = new Texture();
	if (crownTexturePtr->LoadImage("textures/Wrenlyprincecrown1Tex.png") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (meshPtrSkull == nullptr || meshPtrEye == nullptr || meshPtrCrown == nullptr || shaderPtr == nullptr || skullTexturePtr == nullptr || eyeTexturePtr == nullptr || crownTexturePtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}
	leftEye = new DemoObject(skull, meshPtrEye, shaderPtr, eyeTexturePtr);
	if (leftEye == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	rightEye = new DemoObject(skull, meshPtrEye, shaderPtr, eyeTexturePtr);
	if (rightEye == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	crown = new DemoObject(skull, meshPtrCrown, shaderPtr, crownTexturePtr);
	if (crown == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	return true;
}

void Scene2::HandleEvents(const SDL_Event& sdlEvent) {
	trackball->HandleEvents(sdlEvent);
	float mouseCheckX = (sdlEvent.button.x);
	float mouseCheckY = (sdlEvent.button.y);
	if ((270 <= mouseCheckY) && (450 >= mouseCheckY)) {
		mouseY = mouseCheckY;
	}
	if ((530 <= mouseCheckX) && (750 >= mouseCheckX)) {
		mouseX = mouseCheckX;
	}
}

void Scene2::Update(const float deltaTime) {
	Matrix4 studioMaxFix = MMath::rotate(270.0f, Vec3(1.0f, 0.0f, 0.0f));
	skull->setModelMatrix(MMath::translate(skull->getPos()) * MMath::rotate(skull->getAngleDisplacement(), Vec3(0.0f,1.0f,0.0f)));
	crown->setModelMatrix(MMath::translate(0.0f, 2.0f, 0.0f) * MMath::scale(2.0f, 2.0f, 2.0f) * MMath::rotate(11, (Vec3(1.0f, 0.0f, 0.0f))));
	//rightEye->setModelMatrix (MMath::translate(0.55f, 0.2f, 0.8f) * MMath::scale(0.3f,0.3f,0.3f) * MMath::rotate(-90, (Vec3(0.0f, 1.0f, 0.0f))));
	//leftEye->setModelMatrix(MMath::translate(-0.6f, 0.2f, 0.8f) * MMath::scale(0.3f, 0.3f, 0.3f) * MMath::rotate(-90,(Vec3(0.0f,1.0f,0.0f))));

	//Origin Rotate
	rightEye->setModelMatrix(MMath::translate(0.55f, 0.2f, 0.8f) * MMath::scale(0.3f, 0.3f, 0.3f) * MMath::inverse(trackball->getMatrix4()) * MMath::rotate(-90, (Vec3(0.0f, 1.0f, 0.0f))));
	leftEye->setModelMatrix(MMath::translate(-0.6f, 0.2f, 0.8f) * MMath::scale(0.3f, 0.3f, 0.3f) * MMath::inverse(trackball->getMatrix4()) * MMath::rotate(-90, (Vec3(0.0f, 1.0f, 0.0f))));
	//Mouse Rotate
	//rightEye->setModelMatrix(MMath::translate(0.55f, 0.2f, 0.8f) * MMath::scale(0.3f, 0.3f, 0.3f) * MMath::rotate(mouseX, (Vec3(0.0f, 1.0f, 0.0f))) * MMath::rotate(mouseY, (Vec3(0.0f, 0.0f, -1.0f))));
	//leftEye->setModelMatrix(MMath::translate(-0.6f, 0.2f, 0.8f) * MMath::scale(0.3f, 0.3f, 0.3f) * MMath::rotate(mouseX, (Vec3(0.0f, 1.0f, 0.0f))) * MMath::rotate(mouseY, (Vec3(0.0f, 0.0f, -1.0f))));
}

void Scene2::Render() const {
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	/// Draw your scene here
	GLuint program = skull->getShader()->getProgram();
	glUseProgram(program);

	/// These pass the matricies and the light position to the GPU
	glUniformMatrix4fv(skull->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(skull->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix * trackball->getMatrix4());
	glUniform3fv(skull->getShader()->getUniformID("lightPos"), 1, lightPos);
	skull->Render();
	rightEye->Render();
	leftEye->Render();
	crown->Render();
	glUseProgram(0);
}


void Scene2::OnDestroy() {
	if (meshPtrSkull) delete meshPtrSkull, meshPtrSkull = nullptr;
	if (meshPtrEye) delete meshPtrEye, meshPtrEye = nullptr;
	if (skullTexturePtr) delete skullTexturePtr, skullTexturePtr = nullptr;
	if (eyeTexturePtr) delete eyeTexturePtr, eyeTexturePtr = nullptr;
	if (crownTexturePtr) delete crownTexturePtr, crownTexturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (skull) delete skull, skull = nullptr;
	if (rightEye) delete rightEye, rightEye = nullptr;
	if (leftEye) delete leftEye, leftEye = nullptr;
	if (crown) delete crown, crown = nullptr;
}

bool Scene2::skullCreate() {
	//Skull
	if (ObjLoader::loadOBJ("meshes/Skull.obj") == false) {
		return false;
	}
	meshPtrSkull = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	//shaderPtr = new Shader("shaders/toonVert.glsl", "shaders/toonFrag.glsl");
	skullTexturePtr = new Texture();
	if (meshPtrSkull == nullptr || shaderPtr == nullptr || skullTexturePtr == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}
	if (skullTexturePtr->LoadImage("textures/skull_texture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	skull = new DemoObject(nullptr, meshPtrSkull, shaderPtr, skullTexturePtr);
	if (skull == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	return true;
}