#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "DeathScreen.h"
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

DeathScreen::DeathScreen(): camera(nullptr) {
	camera = new Camera();
	//Debug::Info("Created Scene4: ", __FILE__, __LINE__);
}

DeathScreen::~DeathScreen() {
	delete camera;
}

bool DeathScreen::OnCreate() {
	light1 = Vec3(0.0f, 10.0f, 0.0f);
	if (camera->OnCreate() == false) {
		return false;
	}
	if (camera->getSkyBox()->loadTextures("textures/StarSkyboxPosx.png", "textures/StarSkyboxNegx.png", "textures/StarSkyboxPosy.png", "textures/StarSkyboxNegy.png", "textures/StarSkyboxPosz.png", "textures/StarSkyboxNegz.png") == false) {
		return false;
	}
	BuildButtons();
	titleTexture = new Texture();
	titleTexture->LoadImage("textures/deathText.jpg");
	titleBox = new StaticMesh(boxMesh, shaderPtr, titleTexture);
	titleBox->setModelMatrix(MMath::translate(Vec3(0.0, 1.0f, 0.0)) * MMath::scale(3.0f, 1.25f, 0.5f));
	return true;
}

void DeathScreen::HandleEvents(const SDL_Event& sdlEvent) {
	//std::cout << sdlEvent.motion.x << " " << sdlEvent.motion.y << std::endl;
	button1->HandleEvents(sdlEvent);
	//camera->HandleEvents(sdlEvent);
}

int DeathScreen::SetScene() {
	sceneNumber = button1->getScene();
	return sceneNumber;
}

void DeathScreen::BuildButtons() {
	ObjLoader::loadOBJ("meshes/Cube.obj");
	boxMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	endTexture = new Texture();
	endTexture->LoadImage("textures/returnText.jpg");
	shaderPtr = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	button1 = new Button(boxMesh, shaderPtr, endTexture, 2, Vec3(455.0f, 744.0f, 0.0f), Vec3(1123.0f, 548.0f, 0.0f));
	button1->setModelMatrix(MMath::translate(Vec3(0.0, -1.5, 0.0)) * MMath::scale(2.5f, 0.75f, 0.5f));
}

float DeathScreen::setCharacterHealth()
{
	return 50.0f;
}

void DeathScreen::getCharacterHealth(const float stats_) {}

bool DeathScreen::setSceneCleared() {
	return false;
}

void DeathScreen::getSceneCleared(const bool storedRoom_) {}

Vec3 DeathScreen::setCharacterPos() {
	return Vec3(0.0f,5.0f,-15.0f);
}

void DeathScreen::getCharacterPos(const Vec3 storedPos_) {}

float DeathScreen::setCharacterSpeed() {
	return 0.1;
}

void DeathScreen::getCharacterSpeed(const float storedSpeed_) {}

int DeathScreen::setWeapon() {
	return 0;
}

void DeathScreen::getWeapon(const int storedWeapon_) {}

void DeathScreen::Update(const float deltaTime) { }

void DeathScreen::Render() const {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	camera->Render();
	GLuint program = button1->getShader()->getProgram();
	glUseProgram(program);

	glUniformMatrix4fv(button1->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(button1->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
	glUniform3fv(button1->getShader()->getUniformID("lightPos"), 1, light1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, camera->getSkyBox()->getSkyBoxTextureID());
	button1->Render();
	titleBox->Render();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);
}


void DeathScreen::OnDestroy() {
	if (button1) delete button1, button1 = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (endTexture) delete endTexture, endTexture = nullptr;
	if (titleTexture) delete titleTexture, titleTexture = nullptr;
	if (boxMesh) delete boxMesh, boxMesh = nullptr;
}