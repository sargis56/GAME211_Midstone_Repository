#ifndef SCENE_H
#define SCENE_H
#include "SceneManager.h"
#include "CharacterVariables.h"
union SDL_Event;
class Scene {
public:
	explicit Scene() {}
	virtual ~Scene() {}

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const  = 0 ;
	virtual void HandleEvents(const SDL_Event &sdlEvent) = 0;
	//Stat Functions
	virtual CharacterVariables setCharacterVariables() = 0;
	virtual void getCharacterVariables(const CharacterVariables stats_) = 0;
protected:
	CharacterVariables storedStats;
};
#endif