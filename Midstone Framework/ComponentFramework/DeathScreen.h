#ifndef DeathScreen_H
#define DeathScreen_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Trackball.h"
#include "Camera.h"
#include "Button.h"
#include "StaticMesh.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class DemoObject;
class Mesh;
class Shader;
class Texture;

class DeathScreen : public Scene {
private:
	Camera *camera;
	Button* button1;
	Mesh* boxMesh;
	Shader* shaderPtr;
	Texture *endTexture, *titleTexture;
	Vec3 light1;
	StaticMesh* titleBox;
	int sceneNumber = 31;
public:
	explicit DeathScreen();
	virtual ~DeathScreen();

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

	virtual int setWeapon() override;
	virtual void getWeapon(const int storedWeapon_) override;
};


#endif