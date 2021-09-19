#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene5.h"
#include "DemoObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Physics.h"
#include "Timer.h"
#include <SDL_events.h>
#include "Camera.h"
#include "Perlin.h"

Scene5::Scene5(): r1(0.0f), r2(0.0f), b1(0.0f), b2(0.0f), g1(0.0f), g2(0.0f), camera(nullptr), degrees(0.0f), flip(false), noiseTextureID(NULL), sphereObj(nullptr), meshPtrSphereObj(nullptr), shaderPtr(nullptr), texturePtr(nullptr), meshPtrSkullObj(nullptr), skullObj(nullptr), meshPtrVirusObj(nullptr), virusObj(nullptr), totalTime(0.0f), offset(0.0f,0.0f,0.0f), rgb1(0.0f, 0.0f, 0.0f, 0.0f), rgb2(0.0f, 0.0f, 0.0f, 0.0f) {
	trackball = new Trackball();
	camera = new Camera();
	Debug::Info("Created Scene5: ", __FILE__, __LINE__);
}

Scene5::~Scene5() {
	delete camera;
	delete trackball;
}

bool Scene5::OnCreate() {
	noiseTextureID = CreateNoise3D();
	//if (camera->OnCreate() == false) {
	//	return false;
	//}
	if (camera->getSkyBox()->loadTextures("textures/StarSkyboxPosx.png", "textures/StarSkyboxNegx.png", "textures/StarSkyboxPosy.png", "textures/StarSkyboxNegx.png", "textures/StarSkyboxPosz.png", "textures/StarSkyboxNegx.png") == false) {
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
	shaderPtr = new Shader("shaders/timeVert.glsl", "shaders/timeFrag.glsl");
	//shaderPtrYellow = new Shader("shaders/timeVert.glsl", "shaders/timeFragYellow.glsl");

	texturePtr = new Texture();
	if (texturePtr->LoadImage("textures/skull_texture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	if (meshPtrSphereObj == nullptr || shaderPtr == nullptr || texturePtr == nullptr || meshPtrSkullObj == nullptr || meshPtrVirusObj == nullptr) {
		Debug::FatalError("Couldn't create game object assets", __FILE__, __LINE__);
		return false;
	}
	sphereObj = new DemoObject(nullptr, meshPtrSphereObj, shaderPtr, texturePtr);
	if (sphereObj == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	skullObj = new DemoObject(sphereObj, meshPtrSkullObj, shaderPtr, texturePtr);
	if (sphereObj == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	virusObj = new DemoObject(sphereObj, meshPtrVirusObj, shaderPtr, texturePtr);
	if (sphereObj == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	sphereObj->setPos(Vec3(0.0f, 0.0f, 0.0f));
	skullObj->setPos(Vec3(-1.9f, 0.0f, 0.0f));
	virusObj->setPos(Vec3(2.0f, 0.0f, 0.0f));
	return true;
}

void Scene5::HandleEvents(const SDL_Event& sdlEvent) {
	camera->HandleEvents(sdlEvent);
	trackball->HandleEvents(sdlEvent);
}

void Scene5::Update(const float deltaTime) {
	degrees++;
	totalTime += deltaTime;
	offset.x += 0.005f;
	offset.y += 0.005f;
	offset.z += 0.010f;
	//rgb update loop
	
	if (flip == true) {
		r1 -= 0.015f;
		g1 -= 0.015f;
		b1 -= 0.05f; //change to 0.005f for lava
		r2 += 0.001f;
		g2 += 0.02f;
		b2 += 0.005f;
		if (g1 <= 0.1f) {
			flip = false;
		}
	}
	else {
		r1 += 0.015f;
		g1 += 0.015f;
		b1 += 0.05f; //change to 0.005f for lava
		r2 -= 0.001f;
		g2 -= 0.02f;
		b2 -= 0.005f;
		if (g1 >= 1.0f) {
			flip = true;
		}
	}
	
	//set rgbs
	//r1 = 0.8;
	//g1 = 0.7;	
	//b1 = 0.0;
	//r2 = 75;
	//g2 = 0.1;
	//b2 = 130;
	rgb1 = Vec4(r1, g1, b1, 0.0f);
	rgb2 = Vec4(r2, g2, b2, 0.0f);
	sphereObj->setModelMatrix(MMath::translate(sphereObj->getPos()) * MMath::rotate(degrees,Vec3(0.0f,1.0,0.0f)) * MMath::scale(2.0f, 2.0f, 2.0f));
	skullObj->setModelMatrix(MMath::translate(skullObj->getPos()) * MMath::scale(0.2f, 0.2f, 0.2f));
	virusObj->setModelMatrix(MMath::translate(virusObj->getPos()) * MMath::scale(0.5f, 0.5f, 0.5f));
}

void Scene5::Render() const {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	camera->Render();
	GLuint program = sphereObj->getShader()->getProgram();
	glUseProgram(program);

	glUniformMatrix4fv(sphereObj->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(sphereObj->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix() * MMath::transpose(trackball->getMatrix4()));
	//glUniform1f(sphereObj->getShader()->getUniformID("time"), totalTime);
	glUniform3fv(sphereObj->getShader()->getUniformID("offset"),1, offset);
	glUniform4fv(sphereObj->getShader()->getUniformID("rgb1"), 1, rgb1);
	glUniform4fv(sphereObj->getShader()->getUniformID("rgb2"), 1, rgb2);
	glBindTexture(GL_TEXTURE_3D, noiseTextureID);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, camera->getSkyBox()->getSkyBoxTextureID());
	//Sphere Mesh
	sphereObj->Render();
	//Other Meshes
	skullObj->Render();
	virusObj->Render();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindTexture(GL_TEXTURE_3D, 0);
	glUseProgram(0);
}


void Scene5::OnDestroy() {
	if (meshPtrSphereObj) delete meshPtrSphereObj, meshPtrSphereObj = nullptr;
	if (meshPtrSkullObj) delete meshPtrSkullObj, meshPtrSkullObj = nullptr;
	if (meshPtrVirusObj) delete meshPtrVirusObj, meshPtrVirusObj = nullptr;
	if (texturePtr) delete texturePtr, texturePtr = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (sphereObj) delete sphereObj, sphereObj = nullptr;
	if (skullObj) delete skullObj, skullObj = nullptr;
	if (virusObj) delete virusObj, virusObj = nullptr;
	DeleteNoise3D();
}