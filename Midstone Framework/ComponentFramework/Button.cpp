#include "Button.h"
#include "MMath.h"
#include <SDL.h>

Button::Button(Mesh* mesh_, Shader* shader_, Texture* texture_, int function_, Vec3 pos1_, Vec3 pos2_) :
mesh(mesh_), texture(texture_), shader(shader_), pos1(pos1_), pos2(pos2_), function(function_) {
}

Button::~Button() {}

bool Button::OnCreate() {
	return true;
}
void Button::OnDestroy() {}				  /// Just a stub
void Button::Update(float deltaTime_) { }

void Button::Render() const {
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

void Button::HandleEvents(const SDL_Event& event) { 
	if (event.motion.x >= pos1.x && event.motion.x <= pos2.x && event.motion.y <= pos1.y && event.motion.y >= pos2.y) {
		if (event.type == SDL_MOUSEBUTTONDOWN || event.button.button == SDL_BUTTON_LEFT) {
			if (function == 0) {
				//printf("start game\n");
				sceneNumber = 1;
			}
			if (function == 1) {
				//printf("end game\n");
				sceneNumber = 1000;
			}
			if (function == 2) {
				//printf("end game\n");
				sceneNumber = 0;
			}
		}
	}
}