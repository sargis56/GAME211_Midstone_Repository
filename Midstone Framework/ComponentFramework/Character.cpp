#include "Character.h"

Character::Character(Vec3 pos_, Vec3 vel_, Vec3 accel_, float mass_) :
	Object(pos_, vel_, accel_, mass_) {
}

void Character::HandleEvents(const SDL_Event& sdlEvent) {
	if (sdlEvent.type == SDL_KEYDOWN) {
		switch (sdlEvent.key.keysym.scancode) {
		case SDL_SCANCODE_W:
			pos.y = pos.y + 0.1f;
			break;
		case SDL_SCANCODE_D:
			pos.x = pos.x + 0.1f;
			break;
		case SDL_SCANCODE_A:
			pos.x = pos.x - 0.1f;
			break;
		case SDL_SCANCODE_S:
			pos.y = pos.y - 0.1f;
			break;
		default:
			break;
		}

	}
	else if (sdlEvent.type == SDL_MOUSEMOTION) {
		printf("Mouse in motion x= %d y= %d\n", sdlEvent.motion.x, sdlEvent.motion.y);
	}
}

Character::~Character() { }