#ifndef SKYBOX_H
#define SKYBOX_H
#include <glew.h>
#include <SDL_image.h>
#include "GameObject.h"
#include "Mesh.h"
#include "Shader.h"

class SkyBox : public GameObject {
private:
	const char* posX, * negX, * posY, * negY, * posZ, * negZ;
	GLuint skyboxTextureID;
	Mesh* skyboxMesh;
	Shader* skyboxShader;
public:
	//SkyBox();
	SkyBox();
	~SkyBox();
	bool OnCreate() override;
	void OnDestroy() override;
	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;
	bool loadTextures(const char* posX, const char* negX, const char* posY, const char* negY, const char* posZ, const char* negZ);
	Shader* getShader() { return skyboxShader; }
	GLuint getSkyBoxTextureID() { return skyboxTextureID; }
};
#endif // ! SKYBOX_H


