#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H
#include "Vector.h"
#include "Sphere.h"

using namespace MATH;

class PhysicsObject {
	friend class Physics;
protected:
	Sphere sphere;
	Vec3 pos, vel, accel;//, previousAccel;
	float mass;
	Sphere boundingSphere;
	float angle, angularVel, angularAccel, rotationalI;
	//float heightCylinder;
public:
	PhysicsObject();
	virtual ~PhysicsObject();
	void ObjectSetUp(Vec3 pos_, Vec3 vel_, Vec3 accel_, float mass_, float rotationalI_);
public:
	/// Untility functions so small they cane be inlined;
	inline void setPos(const Vec3 &pos_) { pos = pos_; }
	inline Vec3 getPos() { return pos; }
	inline const void setVel(const Vec3 &vel_) { vel = vel_; }
	inline Vec3 getVel() { return vel; }
	inline void setAccel(const Vec3 &accel_) { accel = accel_; }
	inline Vec3 getAccel() { return accel; }
	inline float getAngleDisplacement() { return angle; }
	inline void setAngularVel(const float angularVel_) { angularVel = angularVel_; }
	inline float getAngleVel() { return angularVel; }
	inline float getAngleAccel() { return angularAccel; }
	inline float getRadius() { return sphere.r; }
	inline void setRadius(const float radius_) { sphere.r = radius_; }
	inline void setMass(const float mass_) { mass = mass_; }
	//inline void setHeightCylinder(const float heightCylinder_) { heightCylinder = heightCylinder_; }
	inline void applyForce(const Vec3 force) { accel = force / mass;  }
	void ApplyForceRotation(Vec3 force);
	void ApplyTorque(float torque_);
};
#endif

