#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene4.h"
#include "DemoObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Debug.h"
#include "Physics.h"
#include "Timer.h"
#include <SDL_events.h>
#include "Camera.h"

Scene4::Scene4(): camera(nullptr), sphereObj(nullptr), meshPtrSphereObj(nullptr), shaderPtr(nullptr), texturePtr(nullptr), meshPtrSkullObj(nullptr), skullObj(nullptr), meshPtrVirusObj(nullptr), virusObj(nullptr) {
	trackball = new Trackball();
	camera = new Camera();
	Debug::Info("Created Scene4: ", __FILE__, __LINE__);
}

Scene4::~Scene4() {
	delete camera;
	delete trackball;
}

bool Scene4::OnCreate() {
	if (camera->OnCreate() == false) {
		return false;
	}
	if (camera->getSkyBox()->loadTextures("textures/posx.jpg", "textures/negx.jpg", "textures/posy.jpg", "textures/negy.jpg", "textures/posz.jpg", "textures/negz.jpg") == false) {
		return false;
	}
	if (ObjLoader::loadOBJ("meshes/Sphere.obj") == false) {
		return false;
	}
	meshPtrSphereObj = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	if (ObjLoader::loadOBJ("meshes/Skull.obj") == false) {
		return false;
	}
	meshPtrSkullObj = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	if (ObjLoader::loadOBJ("meshes/CoronaVirus.obj") == false) {
		return false;
	}
	meshPtrVirusObj = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);

	//SHADERS
	shaderPtr = new Shader("shaders/skyboxReflectionVert.glsl", "shaders/skyboxReflectionFrag.glsl"); // Reflection Shader
	//shaderPtr = new Shader("shaders/skyboxReflectionVert.glsl", "shaders/skyboxRefractionFrag.glsl"); //Refraction Shader
	//shaderPtr = new Shader("shaders/skyboxReflectionVert.glsl", "shaders/skyboxFresnelFrag.glsl"); //Fresnel Shader

	texturePtr = new Texture();
	if (texturePtr->LoadImage("textures/skull_texture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (meshPtrSphereObj == nullptr ||  shaderPtr == nullptr || texturePtr == nullptr || meshPtrSkullObj == nullptr || meshPtrVirusObj == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}
	sphereObj = new DemoObject(nullptr, meshPtrSphereObj, shaderPtr, texturePtr);
	if (sphereObj == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	skullObj = new DemoObject(nullptr, meshPtrSkullObj, shaderPtr, texturePtr);
	if (sphereObj == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	virusObj = new DemoObject(nullptr, meshPtrVirusObj, shaderPtr, texturePtr);
	if (sphereObj == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	sphereObj->setPos(Vec3(0.0f, 0.0f, 0.0f));
	skullObj->setPos(Vec3(-4.0f, 0.0f, 0.0f));
	virusObj->setPos(Vec3(4.0f, 0.0f, 0.0f));
	return true;
}

void Scene4::HandleEvents(const SDL_Event& sdlEvent) {
	camera->HandleEvents(sdlEvent);
	trackball->HandleEvents(sdlEvent);
}

void Scene4::Update(const float deltaTime) {
	sphereObj->setModelMatrix(MMath::translate(sphereObj->getPos()));
	skullObj->setModelMatrix(MMath::translate(skullObj->getPos()));
	virusObj->setModelMatrix(MMath::translate(virusObj->getPos()) * MMath::scale(2.0f,2.0f,2.0f));
}

void Scene4::Render() const {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	camera->Render();
	GLuint program = sphereObj->getShader()->getProgram();
	glUseProgram(program);

	glUniformMatrix4fv(sphereObj->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(sphereObj->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix() * MMath::transpose(trackball->getMatrix4()));
	glBindTexture(GL_TEXTURE_CUBE_MAP, camera->getSkyBox()->getSkyBoxTextureID());
	//Sphere Mesh
	sphereObj->Render();
	//Other Meshes
	skullObj->Render();
	virusObj->Render();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);
}


void Scene4::OnDestroy() {
	if (meshPtrSphereObj) delete meshPtrSphereObj, meshPtrSphereObj = nullptr;
	if (meshPtrSkullObj) delete meshPtrSkullObj, meshPtrSkullObj = nullptr;
	if (meshPtrVirusObj) delete meshPtrVirusObj, meshPtrVirusObj = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (sphereObj) delete sphereObj, sphereObj = nullptr;
	if (skullObj) delete skullObj, skullObj = nullptr;
	if (virusObj) delete virusObj, virusObj = nullptr;
}