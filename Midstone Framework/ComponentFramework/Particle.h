#ifndef PARTICLE_H
#define PARTICLE_H
#include <vector>
#include "Vector.h"
#include <glew.h>
using namespace MATH;
class Particle {
public:
	Particle(int numberOfParticles);
	~Particle();
	void Render();
private:
	std::vector<Vec3> pos;
	std::vector<Vec3> vel;
	std::vector<Vec4> color;
	void setupParticles();
	GLuint vao, vbo;
};
#endif