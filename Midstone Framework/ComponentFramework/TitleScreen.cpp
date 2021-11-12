#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "TitleScreen.h"
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

TitleScreen::TitleScreen(): camera(nullptr) {
	camera = new Camera();
	Debug::Info("Created Scene4: ", __FILE__, __LINE__);
}

TitleScreen::~TitleScreen() {
	delete camera;
}

bool TitleScreen::OnCreate() {
	light1 = Vec3(0.0f, 40.0f, 0.0f);
	if (camera->OnCreate() == false) {
		return false;
	}
	if (camera->getSkyBox()->loadTextures("textures/StarSkyboxPosx.png", "textures/StarSkyboxNegx.png", "textures/StarSkyboxPosy.png", "textures/StarSkyboxNegy.png", "textures/StarSkyboxPosz.png", "textures/StarSkyboxNegz.png") == false) {
		return false;
	}
	BuildButtons();
	return true;
}

void TitleScreen::HandleEvents(const SDL_Event& sdlEvent) {
	button1->HandleEvents(sdlEvent);
	button2->HandleEvents(sdlEvent);
	//camera->HandleEvents(sdlEvent);
}

int TitleScreen::SetScene() {
	sceneNumber = button1->getScene();
	return sceneNumber;
}

void TitleScreen::BuildButtons() {
	ObjLoader::loadOBJ("meshes/Cube.obj");
	boxMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	startTexture = new Texture();
	startTexture->LoadImage("textures/green.jpg");
	endTexture = new Texture();
	endTexture->LoadImage("textures/red.jpg");
	shaderPtr = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	button1 = new Button(boxMesh, shaderPtr, startTexture, 0, Vec3(410.0f, 430.0f, 0.0f), Vec3(1190.0f, 235.0f, 0.0f));
	button2 = new Button(boxMesh, shaderPtr, endTexture, 1, Vec3(410.0f, 660.0f, 0.0f), Vec3(1190.0f, 470.0f, 0.0f));
	button1->setModelMatrix(MMath::translate(Vec3(0.0, 0.9, 0.0)) * MMath::scale(3.0f, 0.75f, 0.5f));
	button2->setModelMatrix(MMath::translate(Vec3(0.0, -0.9, 0.0)) * MMath::scale(3.0f, 0.75f, 0.5f));
}

float TitleScreen::setCharacterHealth()
{
	return 50.0f;
}

void TitleScreen::getCharacterHealth(const float stats_) {}

bool TitleScreen::setSceneCleared() {
	return false;
}

void TitleScreen::getSceneCleared(const bool storedRoom_) {}

Vec3 TitleScreen::setCharacterPos() {
	return Vec3(-1.0f,0.0f,-15.0f);
}

void TitleScreen::getCharacterPos(const Vec3 storedPos_) {}

float TitleScreen::setCharacterSpeed() {
	return 0.1;
}

void TitleScreen::getCharacterSpeed(const float storedSpeed_) {}

int TitleScreen::setWeapon() {
	return 0;
}

void TitleScreen::getWeapon(const int storedWeapon_) {}

void TitleScreen::Update(const float deltaTime) { }

void TitleScreen::Render() const {
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
	button2->Render();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);
}


void TitleScreen::OnDestroy() {
	if (button1) delete button1, button1 = nullptr;
	if (button2) delete button2, button2 = nullptr;
	if (button3) delete button3, button3 = nullptr;
	if (startTexture) delete startTexture, startTexture = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (endTexture) delete endTexture, endTexture = nullptr;
	if (boxMesh) delete boxMesh, boxMesh = nullptr;
}