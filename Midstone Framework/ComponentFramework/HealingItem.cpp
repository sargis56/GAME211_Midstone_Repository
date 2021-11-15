#include "HealingItem.h"
#include "MMath.h"
#include "VMath.h"
#include "Physics.h"
#include <SDL.h>
#include <cstdlib>

HealingItem::HealingItem(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_):
	mesh(mesh_), shader(shader_), texture(texture_) {
	pos = pos_;
	itemActive = true;
}

HealingItem::~HealingItem() {}

bool HealingItem::OnCreate() {


	return true; 
}
void HealingItem::OnDestroy() {}				  /// Just a stub
void HealingItem::Update(float deltaTime_) {
}

void HealingItem::Render() const {
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

void HealingItem::HandleEvents(const SDL_Event& event) {
	
} /// Just a stub

bool HealingItem::collisionCheck(Character* character) {
	if (VMath::distance(character->getPos(), pos) < 1) {
		itemActive = false;
		return true;
	}
	return false;
}

