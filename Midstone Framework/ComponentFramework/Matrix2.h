#ifndef MATRIX2_H
#define MATRIX2_H
class Matrix2 {
public:
	float m[4];
	Matrix2();
	Matrix2(float a1_, float a2_, float b1_, float b2_);

	float DotX(float forcex_, float forcey_);

	float DotY(float forcex_, float forcey_);

	//float Dot(float forcex_, float forcey_);

	//Vec3 Dot(Vec3 force_); 

	//Vec3 Dot(Vec3 force_);

};
#endif