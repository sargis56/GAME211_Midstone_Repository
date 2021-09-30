#ifndef SCENE_H
#define SCENE_H
#include "SceneManager.h"
union SDL_Event;
class Scene {
public:
	explicit Scene() { speed = NULL; }
	virtual ~Scene() {}

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const  = 0 ;
	virtual void HandleEvents(const SDL_Event &sdlEvent) = 0;
	//Stat Functions
	virtual float setSpeed() = 0;
	virtual void getSpeed(const float speed_) = 0;
protected:
	float speed;
};
#endif