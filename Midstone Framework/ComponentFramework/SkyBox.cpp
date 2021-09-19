#include "SkyBox.h"
#include <SDL_image.h>
#include "ObjLoader.h"

//SkyBox::SkyBox():skyboxTextureID(0){}
SkyBox::SkyBox() : skyboxTextureID(0) {
	
}
SkyBox::~SkyBox() {
	OnDestroy();
}

bool SkyBox::OnCreate() {
	return true;
}

void SkyBox::OnDestroy() {
	glDeleteTextures(1, &skyboxTextureID);
	if (skyboxMesh) delete skyboxMesh;
	if (skyboxShader) delete skyboxShader;
}

void SkyBox::Update(const float deltaTime) {}

void SkyBox::Render() const {
	if (skyboxTextureID == 0) return;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);

	skyboxMesh->Render();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void SkyBox::HandleEvents(const SDL_Event& event) {}

bool SkyBox::loadTextures(const char* posX, const char* negX, const char* posY, const char* negY, const char* posZ, const char* negZ) {
	if (ObjLoader::loadOBJ("meshes/Cube.obj") == false) {
		return false;
	}
	skyboxMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	if (skyboxMesh == nullptr) {
		return false;
	}
	skyboxShader = new Shader("shaders/skyboxVert.glsl", "shaders/skyboxFrag.glsl");
	if (skyboxShader == nullptr) {
		return false;
	}
	glGenTextures(1, &skyboxTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	SDL_Surface* textureSurface;
	//Pos X
	textureSurface = IMG_Load(posX);
	if (textureSurface == nullptr) {
		return false;
	}
	int mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	//Neg X
	textureSurface = IMG_Load(negX);
	if (textureSurface == nullptr) {
		return false;
	}
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	//Pos Y
	textureSurface = IMG_Load(posY);
	if (textureSurface == nullptr) {
		return false;
	}
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	//Neg Y
	textureSurface = IMG_Load(negY);
	if (textureSurface == nullptr) {
		return false;
	}
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	//Pos Z
	textureSurface = IMG_Load(posZ);
	if (textureSurface == nullptr) {
		return false;
	}
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	//Neg Z
	textureSurface = IMG_Load(negZ);
	if (textureSurface == nullptr) {
		return false;
	}
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return true;
}
