#include "RatEnemy.h"
#include "MMath.h"
#include <SDL.h>
#include <cstdlib>

RatEnemy::RatEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_) :
	mesh(mesh_), shader(shader_), texture(texture_) {
}

RatEnemy::~RatEnemy() {}

bool RatEnemy::OnCreate() { 
	return true; 
}
void RatEnemy::OnDestroy() {}				  /// Just a stub
void RatEnemy::Update(float deltaTime_) {
	if (pos.x > desiredPos.x - 0.01 && pos.x < desiredPos.x + 0.01) {
		pos.x = desiredPos.x;
	}
	if (pos.y > desiredPos.y - 0.01 && pos.y < desiredPos.y + 0.01) {
		pos.y = desiredPos.y;
	}
	if (pos.x == desiredPos.x && pos.y == desiredPos.y) {
		moveOver = true;
	}
	if (moveOver == true) {

		desiredPos = pos;
		direction = rand() % 4;
		if (direction == 0) {
			desiredPos.x = pos.x + 5;
		}
		else if (direction == 1) {
			desiredPos.y = pos.y + 5;
		}
		else if (direction == 2) {
			desiredPos.x = pos.x - 5;
		}
		else {
			desiredPos.y = pos.y - 5;
		}
		moveOver = false;
	}
	if (pos != desiredPos) {
		if (pos.x < desiredPos.x) {
			pos.x = pos.x + 0.1f;
		}
		else if (pos.x > desiredPos.x) {
			pos.x = pos.x - 0.1f;
		}
		else if (pos.y < desiredPos.y) {
			pos.y = pos.y + 0.1f;
		}
		else if (pos.y > desiredPos.y) {
			pos.y = pos.y - 0.1f;
		}
	}
	//printf("desired pos: %f %f %f\n", desiredPos.x, desiredPos.y, desiredPos.z);
}

void RatEnemy::Render() const {
	Matrix3 normalMatrix = MMath::transpose(MMath::inverse(modelMatrix));
	/// This is some fancy code.  Assigning a 4x4 matrix to a 3x3 matrix
	/// just steals the upper 3x3 of the 4x4 and assigns thoses values 
	/// to the 3x3.  
	//Matrix3 normalMatrix = MMath::transpose(MMath::inverse(modelMatrix));
	glUniformMatrix4fv(shader->getUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
	glUniformMatrix3fv(shader->getUniformID("normalMatrix"), 1, GL_FALSE, normalMatrix);
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	}
	mesh->Render();

	/// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RatEnemy::HandleEvents(const SDL_Event& event) {
	
} /// Just a stub
