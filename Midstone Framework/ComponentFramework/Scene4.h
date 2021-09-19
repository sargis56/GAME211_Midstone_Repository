#ifndef Scene4_H
#define Scene4_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Trackball.h"
#include "Camera.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class DemoObject;
class Mesh;
class Shader;
class Texture;

class Scene4 : public Scene {
private:
	Camera *camera;
	DemoObject* sphereObj, *skullObj, *virusObj;
	Trackball* trackball;
	Mesh* meshPtrSphereObj, *meshPtrSkullObj, *meshPtrVirusObj;
	Shader* shaderPtr;
	Texture* texturePtr;
public:
	explicit Scene4();
	virtual ~Scene4();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};


#endif // Scene4_H