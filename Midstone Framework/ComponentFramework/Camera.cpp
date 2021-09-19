#include "Camera.h"
#include "MMath.h"
#include "Trackball.h"
Camera::Camera():skybox(nullptr) {
	//Vec3 pos = Vec3(0.0f, 0.0f, 10.0f);
	projectionMatrix = MMath::perspective(40.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	//viewMatrix = MMath::lookAt(pos, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	translation = MMath::translate(Vec3(0.0f,0.0f,-10.0f));
	rotation = MMath::rotate(0.0f,Vec3(0.0f,1.0f,0.0f));
	viewMatrix = translation * rotation;
	skybox = new SkyBox();
	trackball = new Trackball();
}
Camera::~Camera() {
	delete trackball;
	delete skybox;
}
bool Camera::OnCreate() {
	if (skybox->OnCreate() == false) {
		return false;
	}
	return true;
}
void Camera::OnDestroy() {}
void Camera::Update(const float deltaTime) {}
void Camera::Render() const {
	if (skybox->getSkyBoxTextureID() == 0) return;
	glUseProgram(skybox->getShader()->getProgram());
	glUniformMatrix4fv(skybox->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(skybox->getShader()->getUniformID("cameraRotationMatrix"), 1, GL_FALSE, MMath::transpose(trackball->getMatrix4()));
	skybox->Render();
	glUseProgram(0);
}
void Camera::HandleEvents(const SDL_Event& sdlEvent) {
	trackball->HandleEvents(sdlEvent);
	skybox->HandleEvents(sdlEvent);
	//rotation = trackball->getMatrix4();
	//viewMatrix = translation * rotation;
}