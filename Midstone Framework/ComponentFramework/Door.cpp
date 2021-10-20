#include "Door.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>
#include "ObjLoader.h"

Door::Door(Mesh* mesh_, Shader* shader_, Texture* texture_,Vec3 pos_) :
	mesh(mesh_), shader(shader_), texture(texture_) {
	pos = pos_;
}

Door::~Door() {}

bool Door::OnCreate() {
	return true; 
}
void Door::OnDestroy() {}				  /// Just a stub
void Door::Update(float deltaTime_) {}

void Door::Render() const {
	Matrix3 normalMatrix = MMath::transpose(MMath::inverse(modelMatrix));
	glUniformMatrix4fv(shader->getUniformID("modelMatrix"), 1, GL_FALSE, modelMatrix);
	glUniformMatrix3fv(shader->getUniformID("normalMatrix"), 1, GL_FALSE, normalMatrix);
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	}
	mesh->Render();

	/// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Door::HandleEvents(const SDL_Event& event) {
	
} /// Just a stub

bool Door::CollisionCheck(Character* character) { //function to if the player is inside door
	if (VMath::distance(character->getPos(), pos) < 1) { //if the door is overlaping the player
		return true;
	}
	else {
		return false;
	}
}

