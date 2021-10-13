#ifndef VenomProjectile_H
#define VenomProjectile_H
#include <glew.h>
#include "Matrix.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "PhysicsObject.h"
#include "Room.h"
#include "Character.h"

using namespace MATH;


class VenomProjectile : public PhysicsObject, public GameObject {

private:
	Matrix4 modelMatrix;
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
	bool moveOver = true;
	float direction;
	Room room;

public:
	VenomProjectile(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_, Vec3 pos);
	//RatEnemy(RatEnemy* parent_, Mesh* mesh_, Shader* shader_, Texture* texture_);
	~VenomProjectile();


	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	inline void setOver(const bool& over_) { moveOver = over_; }

	bool VProjectileUpdate(Vec3 direction);
	bool DamageCheck(Character* character);

	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }
};

#endif