#ifndef Scene6_H
#define Scene6_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Trackball.h"
#include "Camera.h"
#include "Particle.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class DemoObject;
class Mesh;
class Shader;
class Texture;

class Scene6 : public Scene {
private:
	Camera *camera;
	Trackball* trackball;
	Shader* shaderPtr;
	float totalTime;
	Particle *particle;
public:
	explicit Scene6();
	virtual ~Scene6();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};


#endif // Scene6_H