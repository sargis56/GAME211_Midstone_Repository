#ifndef SCENE0_H
#define SCENE0_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Light.h"
#include "Character.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class DemoObject;
class Mesh;
class Shader;
class Texture;

class Scene0 : public Scene {
private:
	Character* character;
	//DemoObject* demoObject;
	float numLight;
	Vec3 lightArray[2];
	Light* Light1;

	Mesh* meshPtr;
	Shader* shaderPtr;
	Texture* texturePtr;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
public:
	explicit Scene0();
	virtual ~Scene0();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE0_H