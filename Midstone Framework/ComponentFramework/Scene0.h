#ifndef SCENE0_H
#define SCENE0_H

#include "MMath.h"
#include "Scene.h"
#include <SDL.h>
#include "Character.h"

using namespace MATH;
#define NUM_BALLS 1

class Scene0 : public Scene {
private:
	SDL_Window* window;
	Matrix4 projectionMatrix;
	Character* ballArray[NUM_BALLS];
	SDL_Renderer* renderer;
public:
	explicit Scene0(SDL_Window* sdlWindow);
	virtual ~Scene0();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &sdlEvent) override;
};


#endif // SCENE0_H