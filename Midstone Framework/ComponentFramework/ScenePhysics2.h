#ifndef SCENEPHYSICS2_H
#define SCENEPHYSICS2_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Light.h"
#include "Matrix2.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class DemoObject;
class Mesh;
class Shader;
class Texture;

class ScenePhysics2 : public Scene {
private:
	DemoObject* sphereObject, *sphereObject2;
	float numLight;
	Vec3 lightArray[2];
	Light* Light1;
	Matrix2 rotateMatrix;
	Vec3 force;
	bool engineFail = false;
	int count = 0;
	int time = 0;
	float rads = 0;
	float total = 0;
	Mesh* meshPtr;
	Shader* shaderPtr;
	Texture* texturePtr;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
public:
	explicit ScenePhysics2();
	virtual ~ScenePhysics2();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // ScenePhysics_H