#ifndef TitleScreen_H
#define TitleScreen_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Trackball.h"
#include "Camera.h"
#include "Button.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class DemoObject;
class Mesh;
class Shader;
class Texture;

class TitleScreen : public Scene {
private:
	Camera *camera;
	Button* button1, *button2, *button3;
	Mesh* boxMesh;
	Shader* shaderPtr;
	Texture* startTexture, *endTexture;
	Vec3 light1;
public:
	explicit TitleScreen();
	virtual ~TitleScreen();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;

	void BuildButtons();

	virtual float setCharacterVariables() override;
	virtual void getCharacterVariables(const float stats_) override;
};


#endif