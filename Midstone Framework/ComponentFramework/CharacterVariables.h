#ifndef CHARACTERVARIABLES_H
#define CHARACTERVARIABLES_H
#include "Vector.h"
using namespace MATH;

class CharacterVariables {
private:
	Vec3 position;
	float health;
	float speed;
public:
	CharacterVariables();
	~CharacterVariables();

	inline Vec3 getPosition() const { return position; }
	inline float getHealth() const { return health; }
	inline float getSpeed() const { return speed; }
	inline void setPosition(const Vec3 position_) { position = position_; }
	inline void setHealth(const float health_) { health = health_; }
	inline void setSpeed(const float speed_) { speed = speed_; }
};
#endif