#ifndef CAMERA_H
#define CAMERA_H
#include "GameObject.h"
#include "Matrix.h"
#include "Skybox.h"
using namespace MATH;
class Camera : public GameObject {
private:
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Matrix4 rotation;
	Matrix4 translation;
	SkyBox *skybox;
	class Trackball *trackball;
public:
	Camera();
	~Camera();
	bool OnCreate() override;
	void OnDestroy() override;
	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;
	Matrix4 getProjectionMatrix() { return projectionMatrix; }
	Matrix4 getViewMatrix() { return viewMatrix; }
	//Matrix4 getRotationMatrix() { return trackball->getMatrix4(); }
	SkyBox* getSkyBox() { return skybox; }
};
#endif