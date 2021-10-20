#include "HealthUI.h"
#include "ObjLoader.h"
#include "MMath.h"
#include <SDL.h>
#include "Character.h"

HealthUI::HealthUI(Mesh* mesh_, Shader* shader_, Texture* texture_) :
	mesh(mesh_), shader(shader_), texture(texture_) {
}

HealthUI::~HealthUI() {}

bool HealthUI::OnCreate()
{
	return true;
}

void HealthUI::OnDestroy(){}

void HealthUI::Update(const float deltaTime_)
{
}

void HealthUI::Render() const
{
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

void HealthUI::HandleEvents(const SDL_Event& event)
{
}
