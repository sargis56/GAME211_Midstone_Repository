#include "Particle.h"
#include "Randomizer.h"
#define POS_LENGTH 	(pos.size() * (sizeof(Vec3)))
#define VEL_LENGTH 	(vel.size() * (sizeof(Vec3)))
#define COLOR_LENGTH (color.size() * (sizeof(Vec4)))
Particle::Particle(int numberOfParticles) {
	Randomizer r;
	for (int i = 0; i < numberOfParticles; i++) {
		pos.push_back(Vec3(0.0f,0.0f,0.0f));
		vel.push_back(Vec3(r.box_muller(0.0f, 2.0f), r.box_muller(0.0f, 2.0f), r.box_muller(0.0f, 2.0f)));
		color.push_back(Vec4(r.rand(0.0f,1.0f), r.rand(0.0f, 1.0f), r.rand(0.0f, 1.0f), 0.0f));
	}
	setupParticles();
}
Particle::~Particle() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}
void Particle::Render() {
	glEnable(GL_PROGRAM_POINT_SIZE);
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, pos.size());
	glBindVertexArray(0); // Disable the VAO
	glDisable(GL_PROGRAM_POINT_SIZE);
}
void Particle::setupParticles() {
	const int posID = 0;
	const int velID = 1;
	const int colorID = 2;
	/// create and bind the VOA
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	/// Create and initialize vertex buffer object VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, POS_LENGTH + VEL_LENGTH + COLOR_LENGTH, NULL, GL_STATIC_DRAW);
	/// assigns the addr of "vectors" to be the beginning of the array buffer "sizeof(vectors)" in length
	glBufferSubData(GL_ARRAY_BUFFER, 0, POS_LENGTH, &pos[0]);
	/// assigns the addr of "velocity" to be "sizeof(vectors)" offset from the beginning and "sizeof(velocity)" in length  
	glBufferSubData(GL_ARRAY_BUFFER, POS_LENGTH, VEL_LENGTH, &vel[0]);
	/// assigns the addr of "color" to be "sizeof(vectors) + sizeof(velocity)" offset from the beginning and "sizeof(color)" in length  
	glBufferSubData(GL_ARRAY_BUFFER, POS_LENGTH + VEL_LENGTH, COLOR_LENGTH, &color[0]);

	glEnableVertexAttribArray(posID);
	glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(velID);
	glVertexAttribPointer(velID, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(POS_LENGTH));
	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(POS_LENGTH + VEL_LENGTH));
	glBindVertexArray(0); //Unbind
}
#undef POS_LENGTH
#undef VEL_LENGTH
#undef COLOR_LENGTH