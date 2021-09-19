#ifndef OBJECT_H
#define OBJECT_H

#include "Vector.h"
#include <SDL_image.h>
using namespace MATH;

class Object {
protected:
	Vec3 pos;
	Vec3 vel;
	Vec3 accel;
	float mass;
	SDL_Surface* image;
	Vec3 imageSizeWorldCoords;
	SDL_Texture* texture;
	double rotation;
	bool select;

public:
	Object();
	Object(Vec3 pos_, Vec3 vel_, Vec3 accel_, float mass_);
	~Object();
	void Update(float deltaTime);
	void HandleEvents(const SDL_Event& sdlEvent);
	void ApplyForce(Vec3 force_);
	Vec3 getPos() { return pos; }
	float getMass() { return mass; }
	Vec3 getVel() { return vel; }
	void setPos(Vec3 pos_) { pos = pos_; }
	void setImage(SDL_Surface* image_) { image = image_; }
	SDL_Surface* getImage() { return image; }
	void setImageSizeWorldCoords(Vec3 imageSizeWorldCoords_) {
		imageSizeWorldCoords.x = imageSizeWorldCoords_.x;
		imageSizeWorldCoords.y = imageSizeWorldCoords_.y;
		imageSizeWorldCoords.z = imageSizeWorldCoords_.z;
	}
	Vec3 getImageSizeWorldCoords() { return imageSizeWorldCoords; }
	void setVel(Vec3 vel_) { vel = vel_; }
	double getRotation() { return rotation; }
	void setTexture(SDL_Texture* texture_) { texture = texture_; }
	SDL_Texture* getTexture() { return texture; }
	float getRadius() { return imageSizeWorldCoords.x / 2.0f; }
	bool getSelected() { return select; }
	void selected() { select = true; }
	void deselected() { select = false; }
};

#endif