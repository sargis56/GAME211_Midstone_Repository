#include "Matrix2.h"
//#include "ScenePhysics.h"

Matrix2::Matrix2() {
	m[0] = 0.0f;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
}
Matrix2::Matrix2(float a1_, float a2_, float b1_, float b2_) {
	m[0] = a1_;
	m[1] = a2_;
	m[2] = b1_;
	m[3] = b2_;
}

float Matrix2::DotX(float forcex_,float forcey_) {
	float x = (m[0] * forcex_) - (m[1] * forcey_);
	//float y = (m[2] * forcex_) + (m[3] * forcey_);
	return(x);
}
float Matrix2::DotY(float forcex_, float forcey_) {
	//float x = (m[0] * forcex_) - (m[1] * forcey_);
	float y = (m[2] * forcex_) + (m[3] * forcey_);
	return(y);
}