#ifndef ROOM_H
#define ROOM_H
#include <glew.h>
#include "Matrix.h"

using namespace MATH;

class Room {

private:
	Vec3 corner1, corner2;
public:
	Room();
	Room(Vec3 corner1_, Vec3 corner2_);
	~Room();
	bool InsideCollisionNegX(Vec3 pos_, float border);
	bool InsideCollisionPosX(Vec3 pos_, float border);
	bool InsideCollisionNegY(Vec3 pos_, float border);
	bool InsideCollisionPosY(Vec3 pos_, float border);
	void setRoomBorders(Vec3 corner1_, Vec3 corner2_);
};

#endif