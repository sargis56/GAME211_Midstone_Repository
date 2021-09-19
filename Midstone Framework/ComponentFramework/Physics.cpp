#include "Physics.h"
#include "VMath.h"
#include <iostream>
#include <cmath>
#include <algorithm>

void Physics::SimpleNewtonMotion(PhysicsObject &object, const float deltaTime) {
	object.pos += object.vel * deltaTime + 0.5f * object.accel * deltaTime * deltaTime;
	//object.pos += object.vel * deltaTime + object.accel * (deltaTime * deltaTime) * 0.5;
	object.vel += object.accel * deltaTime;
}

void Physics::RigidBodyRotation(PhysicsObject& object, const float deltaTime) {
	object.angle += object.angularVel * deltaTime + 0.5f * object.angularAccel * deltaTime * deltaTime;
	//object.angle += object.angularVel * deltaTime + object.angularAccel * (deltaTime * deltaTime) * 0.5;
	object.angularVel += object.angularAccel * deltaTime;
}

bool Physics::PlaneSphereCollision(PhysicsObject &object, Plane &p) {
	float distance = p.distance(object);
	if (distance <= object.sphere.r) {
		return true;
	}
	else {
		return false;
	}
}

void Physics::PlaneSphereCollisionResponse(PhysicsObject &object, Plane &p) {
	if (PlaneSphereCollision(object,p)) {
		//Vec3 normal = Vec3(p.y, -p.x, p.z);
		Vec3 normal = Vec3(p.x, p.y, p.z);
		normal.Normalize();
		Vec3 projectionVector = (-object.vel).Dot(normal) * normal;
		object.vel = object.vel + (2 * projectionVector);
	}
}

bool Physics::SphereSphereCollision(const PhysicsObject &object1, const PhysicsObject &object2) {
	Vec3 dir = object1.pos - object2.pos;
	float mag = dir.Mag();
	if (mag < (object1.sphere.r + object2.sphere.r)) {
		return true;
	}
	else {
		return false;
	}
	/*
	float dist = sqrt(pow(2, (object2.pos.x - object1.pos.x)) + pow(2, (object2.pos.y - object1.pos.y)) + pow(2, (object2.pos.z - object1.pos.z)));
	if (dist < (object2.sphere.r + object1.sphere.r)) {
		return true;
	}
	else {
		return false;
	}*/
}

void Physics::SphereSphereCollisionResponse(PhysicsObject &object1, PhysicsObject &object2, float CR) {
	if (SphereSphereCollision(object1, object2)) {
		//find normal and normalize it
		Vec3 normal;
		normal = object1.pos - object2.pos;
		normal.Normalize();
		//Vec3 projectionVector1 = (-object1.vel).Dot(normal) * normal;
		//find the projection mag
		float projMag1 = -object1.vel.Dot(normal);
		normal = object2.pos - object1.pos;
		normal.Normalize();
		//Vec3 projectionVector2 = (-object2.vel).Dot(normal) * normal;
		float projMag2 = -object2.vel.Dot(normal);
		//Conservation of Momentum equation substituted into Coefficient of Restitution equation
		float CR1 = ((object1.mass - CR * object2.mass) * (1 + CR) * object2.mass * projMag2) / (object1.mass + object2.mass);
		float CR2 = ((object2.mass - CR * object1.mass) * (1 + CR) * object1.mass * projMag1) / (object1.mass + object2.mass);
		object1.vel = (object1.vel + (CR1 - projMag1) * normal);
		object2.vel = (object2.vel + (CR2 - projMag2) * normal);
		//object1.vel = object1.vel + (2 * projectionVector1);
		//object2.vel = object2.vel + (2 * projectionVector2);
	}
}

void Physics::SphereStaticSphereCollisionResponse(PhysicsObject& object1, PhysicsObject& object2, float CR) {
	if (SphereSphereCollision(object1, object2)) {
		Vec3 normal;
		Vec3 projectionVector;
		normal = object1.pos - object2.pos;
		normal.Normalize();
		projectionVector = (-object1.vel).Dot(normal) * normal;
		//Vec3 projectionVector2 = (-object2.vel).Dot(normal) * normal;
		//float projMag1 = -object1.vel.Dot(normal);
		//normal = object2.pos - object1.pos;
		//normal.Normalize();
		//float projMag2 = -object2.vel.Dot(normal);
		//float CR1 = ((object1.mass - CR * object2.mass) * (1 + CR) * object2.mass * projMag2) / (object1.mass * object2.mass);
		//object1.vel = (object1.vel + (CR1 - projMag1) * normal);
		object1.vel = object1.vel + (2 * projectionVector);
	}
}

void Physics::BuoyancyApplyForces(PhysicsObject& object, float waterHeight, float dragCoefficient, float liquidDensity, float length) {
	float gravitationalAcceleration = 9.81f;
	Vec3 weightForce = object.mass * Vec3(0.0f, -gravitationalAcceleration, 0.0f);
	Vec3 dragForce = -dragCoefficient * object.getVel();
	//Vec3 dragForce;
	float mediumDensity = liquidDensity;
	float volume = 0.0f;
	if ((object.pos.y + object.sphere.r) <= waterHeight) {
		volume = M_PI * object.sphere.r * object.sphere.r * length;
		mediumDensity = liquidDensity;
		//printf("Under Full Amount\n");
	}
	else if ((object.pos.y - object.sphere.r) <= waterHeight && object.pos.y >= waterHeight) {
		float disToWater = abs(object.pos.y - waterHeight);
		float theta = 2 * acos(std::clamp(disToWater / object.sphere.r, -1.0f, 1.0f));
		mediumDensity = liquidDensity;
		volume = 0.5 * object.sphere.r * object.sphere.r * (theta - sin(theta)) * length;
		//printf("Under Small Amount\n");
	}
	else if ((object.pos.y) < waterHeight) {
		float disToWater = abs(object.pos.y - waterHeight);
		float theta = 2 * acos(std::clamp(disToWater / object.sphere.r, -1.0f, 1.0f));
		volume = (M_PI * object.sphere.r * object.sphere.r * length) - (0.5 * object.sphere.r * object.sphere.r) * (theta - sin(theta)) * length;
		mediumDensity = liquidDensity;
		//printf("Under Large Amount\n");
	}
	else if (waterHeight < object.pos.y - object.sphere.r) { //Air
		//volume = 0.0f;
		//mediumDensity = 1.225f;
		//printf("In Air\n");
	}
	else {
		printf("ERROR\n");
	}
	Vec3 buoyancyForce = mediumDensity * Vec3(0.0f, gravitationalAcceleration, 0.0f) * volume;
	object.accel = ((weightForce + dragForce + buoyancyForce)/ object.mass);
}

void Physics::VerletMotion(PhysicsObject& object, const float deltaTime, Vec3 previousAccel) {
	object.pos += object.vel * deltaTime + (0.5f * previousAccel * deltaTime * deltaTime);
	object.vel += 0.5f * (previousAccel + object.accel) * deltaTime; //0.3f
}