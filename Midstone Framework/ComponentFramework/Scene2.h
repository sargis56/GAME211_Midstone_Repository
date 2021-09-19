#ifndef SCENE2_H
#define SCENE2_H
#define forever for(;;)
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Trackball.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class DemoObject;
class Mesh;
class Shader;
class Texture;

class Scene2 : public Scene {
private:
	DemoObject* skull;
	DemoObject* leftEye, * rightEye, * crown;
	Trackball* trackball;
	Vec3 lightPos;
	float mouseX, mouseY;
	Mesh* meshPtrSkull, * meshPtrEye, * meshPtrCrown;
	Shader* shaderPtr;
	Texture* skullTexturePtr, * eyeTexturePtr, * crownTexturePtr;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;

	bool skullCreate();
public:
	explicit Scene2();
	virtual ~Scene2();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};


#endif // SCENE0_H