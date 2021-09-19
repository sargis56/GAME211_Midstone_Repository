#include "Object.h"

Object::Object() {
	pos.x = 0.0f;
	pos.y = 0.0f;
	pos.z = 0.0f;

	vel.x = 4.0f;
	vel.y = 0.0f;
	vel.z = 0.0f;

	accel.x = 0.0f;
	accel.y = 0.0f;
	accel.z = 0.0f;

	mass = 1.0f;
	rotation = 0.0;
	image = nullptr;
}

Object::Object(Vec3 pos_, Vec3 vel_, Vec3 accel_, float mass_) :
	pos(pos_), vel(vel_), accel(accel_), mass(mass_), image(nullptr), rotation(0.0), select(false) {

}

Object::~Object() {

}

void Object::ApplyForce(Vec3 force_) {
	accel.x = force_.x / mass;
	accel.y = force_.y / mass;
	accel.z = force_.z / mass;
}

void Object::HandleEvents(const SDL_Event& sdlEvent) {

}

void Object::Update(float deltaTime) {
	pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;
	vel += accel * deltaTime;

	rotation += (360.0f * vel.x * deltaTime) / (2.0f * M_PI * imageSizeWorldCoords.x / 2.0f);
	/*
		if (pos.y - getRadius() < -7.5f) { // floor
			if (vel.y < 0.0f) {
				vel.y *= -1.0f;
			}
		}

		if (pos.x - getRadius() < -15.0f) { // left wall
			if (vel.x < 0.0f) {
				vel.x *= -1.0f;
			}
		}

		if (pos.x + getRadius() > 15.0f) { //right wall
			if (vel.x > 0.0f) {
				vel.x *= -1.0f;
			}
		}
		if (pos.y + getRadius() > 7.5f) { //roof
			if (vel.y > 0.0f) {
				vel.y *= -1.0f;
			}
		}*/
}
