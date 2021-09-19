#ifndef ScenePhysics3_H
#define ScenePhysics3_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Light.h"
#include "Matrix2.h"
#include "Plane.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class DemoObject;
class Mesh;
class Shader;
class Texture;

class ScenePhysics3 : public Scene {
private:
	DemoObject* demoObject, *demoObject2;
	float numLight;
	Vec3 lightArray[2];
	Light* Light1;
	Matrix2 rotateMatrix;
	Vec3 force;
	Plane planeMain;
	bool engineFail = false;
	int count = 0;
	int time = 0;
	float rads = 0;
	float total = 0;
	Mesh* meshPtr , *cubeMeshPtr;
	Shader* shaderPtr;
	Texture* texturePtr;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
public:
	explicit ScenePhysics3();
	virtual ~ScenePhysics3();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // ScenePhysics_H