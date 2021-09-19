#ifndef PHYSICS_H
#define PHYSICS_H
#include "PhysicsObject.h"
#include "Plane.h"
#include <iostream>
class Physics {
public:
	static void SimpleNewtonMotion(PhysicsObject &object, const float deltaTime);
	static void RigidBodyRotation(PhysicsObject& object, const float deltaTime);
	static bool PlaneSphereCollision(PhysicsObject &object, Plane &p);
	static void PlaneSphereCollisionResponse(PhysicsObject &object, Plane &p);
	static bool SphereSphereCollision(const PhysicsObject &object1, const PhysicsObject &object2);
	static void SphereSphereCollisionResponse(PhysicsObject& object1, PhysicsObject& object2, float CR);
	static void SphereStaticSphereCollisionResponse(PhysicsObject& object1, PhysicsObject& object2, float CR);
	static void BuoyancyApplyForces(PhysicsObject& object, float waterHeight, float dragCoefficient, float liquidDensity, float length);
	static void VerletMotion(PhysicsObject& object, const float deltaTime, Vec3 previousAccel);
};

#endif