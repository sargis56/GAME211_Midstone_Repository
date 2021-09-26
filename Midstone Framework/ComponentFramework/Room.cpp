#include "Room.h"

#include "MMath.h"
#include <SDL.h>

Room::Room() {
	corner1 = Vec3(-1.0f, -1.0f, 0.0f);
	corner2 = Vec3(1.0f, 1.0f, 0.0f);
}

Room::Room(Vec3 corner1_, Vec3 corner2_) {
	corner1 = corner1_;
	corner2 = corner2_;
}

Room::~Room() { }

bool Room::InsideCollisionNegX(Vec3 pos_, float border) { // if -pos.x is inside the border return true, else return false
	if (corner1.x < (pos_.x - border)) {
		return true;
	}
	return false;
}

bool Room::InsideCollisionPosX(Vec3 pos_, float border) { // if pos.x is inside the border return true, else return false
	if (corner2.x > (pos_.x + border)) {
		return true;
	}
	return false;
}

bool Room::InsideCollisionNegY(Vec3 pos_, float border) {// if -pos.y is inside the border return true, else return false
	if (corner1.y < (pos_.y - border)) {
		return true;
	}
	return false;
}

bool Room::InsideCollisionPosY(Vec3 pos_, float border) {// if pos.y is inside the border return true, else return false
	if (corner2.y > (pos_.y + border)) {
		return true;
	}
	return false;
}

void Room::setRoomBorders(Vec3 corner1_, Vec3 corner2_) {
	corner1 = corner1_;
	corner2 = corner2_;
}
