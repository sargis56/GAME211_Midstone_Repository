#ifndef BUTTON_H
#define BUTTON_H
#include <glew.h>
#include "Matrix.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "PhysicsObject.h"
#include "Room.h"
#include "SceneManager.h"

using namespace MATH;


class Button : public GameObject {

private:
	Matrix4 modelMatrix;
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
	Vec3 pos1, pos2;
	int function;
	int sceneNumber = -1;
public:
	Button(Mesh* mesh_, Shader* shader_, Texture* texture_, int function_, Vec3 pos1_, Vec3 pos2_);
	~Button();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	inline int getScene() const { return sceneNumber; }
	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }
};

#endif