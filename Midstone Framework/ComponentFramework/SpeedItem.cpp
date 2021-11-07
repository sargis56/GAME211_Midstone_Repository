#include "SpeedItem.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>

SpeedItem::SpeedItem(Mesh* mesh_, Shader* shader_, Texture* texture_, float speedIncrease_, Vec3 pos_):
	mesh(mesh_), shader(shader_), texture(texture_), speedIncrease(speedIncrease_) {
	pos = pos_;
	itemActive = true;
}

SpeedItem::~SpeedItem() {}

bool SpeedItem::OnCreate() {


	return true; 
}
void SpeedItem::OnDestroy() {}				  /// Just a stub
void SpeedItem::Update(float deltaTime_) {
}

void SpeedItem::Render() const {
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

void SpeedItem::HandleEvents(const SDL_Event& event) {
	
} /// Just a stub

void SpeedItem::collisionCheck(Character* character) {
	if (VMath::distance(character->getPos(), pos) < 1) {
		character->setSpeed(speedIncrease);
		itemActive = false;
	}
}

