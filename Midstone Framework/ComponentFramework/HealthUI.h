#ifndef HEALTHUI_H
#define HEALTHUI_H
#include <glew.h>
#include "Matrix.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "PhysicsObject.h"

using namespace MATH;

class HealthUI : public PhysicsObject, public GameObject {

private:
	Matrix4 modelMatrix;
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
	int health, maxHealth;

public:
	HealthUI(Mesh* mesh_, Shader* shader_, Texture* texture_);
	~HealthUI();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	inline Mesh* getMesh() const { return mesh; }
	inline Texture* getTexture() const { return texture; }
	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }
};

#endif