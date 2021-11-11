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

	int sceneNumber = 0;
public:
	explicit TitleScreen();
	virtual ~TitleScreen();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
	virtual int SetScene() override;

	void BuildButtons();

	virtual bool setSceneCleared() override;
	virtual void getSceneCleared(const bool storedScene_) override;

	virtual float setCharacterHealth() override;
	virtual void getCharacterHealth(const float stats_) override;

	virtual Vec3 setCharacterPos() override;
	virtual void getCharacterPos(const Vec3 storedPos_) override;

	virtual float setCharacterSpeed() override;
	virtual void getCharacterSpeed(const float storedSpeed_) override;
};


#endif