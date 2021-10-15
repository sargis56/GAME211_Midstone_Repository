#ifndef SCENE_H
#define SCENE_H
#include "SceneManager.h"
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
	//Stat Functions
	virtual float setCharacterVariables() = 0;
	virtual void getCharacterVariables(const float storedHealth) = 0;
protected:
	float storedHealth;
};
#endif