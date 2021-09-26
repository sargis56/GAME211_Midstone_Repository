#include "StaticMesh.h"
#include "MMath.h"
#include <SDL.h>

StaticMesh::StaticMesh(Mesh* mesh_, Shader* shader_, Texture* texture_) :
mesh(mesh_), texture(texture_), shader(shader_) {
}

StaticMesh::~StaticMesh() {}

bool StaticMesh::OnCreate() {
	return true;
}
void StaticMesh::OnDestroy() {}				  /// Just a stub
void StaticMesh::Update(float deltaTime_) { }

void StaticMesh::Render() const {
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

void StaticMesh::HandleEvents(const SDL_Event& event) { } /// Just a stub