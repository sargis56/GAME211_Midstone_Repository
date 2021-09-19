#ifndef Character_H
#define Character_H_H
#include "Object.h"
class Character : public Object {
public:
	Character(Vec3 pos_, Vec3 vel_, Vec3 accel_, float mass_);
	~Character();
	void HandleEvents(const SDL_Event& sdlEvent);
};

#endif