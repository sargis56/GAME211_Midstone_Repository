#ifndef ScenePhysics4_H
#define ScenePhysics4_H
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

class ScenePhysics4 : public Scene {
private:
	DemoObject* logObject, *waterObject;
	Vec3 lightPos;
	Vec3 previousAccel;
	Matrix2 rotateMatrix;
	Vec3 force;
	Mesh* meshPtr , *cubeMeshPtr;
	Shader* shaderPtr;
	Texture* textureLogPtr, *textureWaterPtr;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
public:
	explicit ScenePhysics4();
	virtual ~ScenePhysics4();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // ScenePhysics_H