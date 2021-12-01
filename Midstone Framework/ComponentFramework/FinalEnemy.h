#ifndef FINALENEMY_H
#define FINALENEMY_H
#include <glew.h>
#include "Matrix.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "PhysicsObject.h"
#include "Room.h"
#include "Character.h"
#include "ProjectileDynamic.h"
#include "Projectile.h"

using namespace MATH;


class FinalEnemy : public PhysicsObject, public GameObject {

private:
	Matrix4 modelMatrix;
	Mesh* mesh, * meshProjectile;
	Shader* shader, * shaderProjectile;
	Texture* texture, *textureProjectile;
	Room room;
	ProjectileDynamic* projectile1;
	Projectile* projectile2, * projectile3, * projectile4, * projectile5;
	Character* character;
	Vec3 projectileDestination;
	int attackPattern = 1;
public:
	FinalEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_, Character* character);
	//RatEnemy(RatEnemy* parent_, Mesh* mesh_, Shader* shader_, Texture* texture_);
	~FinalEnemy();
	void BuildProjectile();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	bool DamageCheck(Character* character);

	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }
};

#endif