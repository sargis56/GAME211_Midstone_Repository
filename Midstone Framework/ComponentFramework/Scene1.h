#ifndef SCENE1_H
#define SCENE1_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Character.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class DemoObject;
class Mesh;
class Shader;
class Texture;

class Scene1 : public Scene {
private:
	Character* character;
	//DemoObject* demoObject;
	float numLight;
	Vec3 lightArray[2];

	Mesh* meshPtr;
	Shader* shaderPtr;
	Texture* texturePtr;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	float speed;
public:
	explicit Scene1();
	virtual ~Scene1();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;

	virtual float setSpeed() override;
	virtual void getSpeed(const float speed_) override;
};


#endif