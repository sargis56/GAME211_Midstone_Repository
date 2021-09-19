#include "PhysicsObject.h"
#include "MMath.h"

PhysicsObject::PhysicsObject():pos(0.0f),vel(0.0f),accel(0.0f),mass(1.0f),boundingSphere(0.0f){
	/// Another way to initialize within the constructor
	angle = angularVel = angularAccel = 0.0f;
	rotationalI = 1.0f;

}


PhysicsObject::~PhysicsObject(){

}

void PhysicsObject::ObjectSetUp(Vec3 pos_, Vec3 vel_, Vec3 accel_, float mass_, float rotationalI_) {
	pos = pos_;
	vel = vel_;
	accel = accel_;
	mass = mass_;
	rotationalI = rotationalI_;
}

void PhysicsObject::ApplyForceRotation(Vec3 force) {
	//force.RotateZ(angle);
	accel = force / mass;
}

void PhysicsObject::ApplyTorque(float torque) {
	angularAccel = torque / rotationalI;
}

