#ifndef Scene5_H
#define Scene5_H
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

class Scene5 : public Scene {
private:
	Camera *camera;
	DemoObject* sphereObj, *skullObj, *virusObj;
	Trackball* trackball;
	Mesh* meshPtrSphereObj, *meshPtrSkullObj, * meshPtrVirusObj;
	Shader* shaderPtr;
	Texture* texturePtr;
	GLuint noiseTextureID;
	float totalTime, r1, g1, b1, r2, g2, b2;
	Vec3 offset; 
	Vec4 rgb1, rgb2;
	float degrees;
	bool flip;
public:
	explicit Scene5();
	virtual ~Scene5();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};


#endif // Scene5_H