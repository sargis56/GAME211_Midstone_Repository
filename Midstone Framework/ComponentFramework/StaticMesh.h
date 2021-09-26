#ifndef STATICMESH_H
#define STATICMESH_H
#include <glew.h>
#include "Matrix.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "PhysicsObject.h"
#include "Room.h"

using namespace MATH;


class StaticMesh : public GameObject {

private:
	Matrix4 modelMatrix;
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
public:
	StaticMesh(Mesh* mesh_, Shader* shader_, Texture* texture_);
	~StaticMesh();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;


	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }
};

#endif