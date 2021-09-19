#include <glew.h>
#include <iostream>
#include "Debug.h"
#include "Scene6.h"
#include "Shader.h"
#include "MMath.h"
#include "Debug.h"
#include "Physics.h"
#include "Timer.h"
#include <SDL_events.h>
#include "Camera.h"

Scene6::Scene6(): camera(nullptr), shaderPtr(nullptr), totalTime(0.0f) {
	trackball = new Trackball();
	camera = new Camera();
	Debug::Info("Created Scene6: ", __FILE__, __LINE__);
}

Scene6::~Scene6() {
	delete camera;
	delete trackball;
}

bool Scene6::OnCreate() {
	particle = new Particle(999999);
	if (camera->OnCreate() == false) {
		return false;
	}
	if (camera->getSkyBox()->loadTextures("textures/StarSkyboxPosx.png", "textures/StarSkyboxNegx.png", "textures/StarSkyboxPosy.png", "textures/StarSkyboxNegx.png", "textures/StarSkyboxPosz.png", "textures/StarSkyboxNegx.png") == false) {
		return false;
	}

	//SHADERS
	shaderPtr = new Shader("shaders/particleVert.glsl", "shaders/particleFrag.glsl");
	return true;
}

void Scene6::HandleEvents(const SDL_Event& sdlEvent) {
	camera->HandleEvents(sdlEvent);
	trackball->HandleEvents(sdlEvent);
}

void Scene6::Update(const float deltaTime) {
	totalTime += deltaTime/10.0f;
}

void Scene6::Render() const {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	camera->Render();
	glUseProgram(shaderPtr->getProgram());
	glUniformMatrix4fv(shaderPtr->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(shaderPtr->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix() * MMath::transpose(trackball->getMatrix4()));
	glUniform1f(shaderPtr->getUniformID("totalTime"), totalTime);
	particle->Render();
	glUseProgram(0);
}


void Scene6::OnDestroy() {
	if (particle) delete particle, particle = nullptr;
}