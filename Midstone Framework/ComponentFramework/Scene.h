#ifndef SCENE_H
#define SCENE_H
#include "SceneManager.h"
#include "Vector.h"
using namespace MATH;
union SDL_Event;
class Scene {
public:
	explicit Scene() { storedHealth = NULL; }
	virtual ~Scene() {}

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const  = 0 ;
	virtual void HandleEvents(const SDL_Event &sdlEvent) = 0;
	virtual int SetScene() = 0; //function to switch scenes
	//Stat Functions
	virtual float setCharacterHealth() = 0;
	virtual void getCharacterHealth(const float storedHealth_) = 0;
	virtual Vec3 setCharacterPos() = 0;
	virtual void getCharacterPos(const Vec3 storedPos_) = 0;
protected:
	float storedHealth;
	Vec3 storedPos;
};
#endif