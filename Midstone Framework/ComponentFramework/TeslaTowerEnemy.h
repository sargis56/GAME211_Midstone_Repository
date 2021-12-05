#ifndef TeslaTowerEnemy_H
#define TeslaTowerEnemy_H
#include <glew.h>
#include "Matrix.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "PhysicsObject.h"
#include "Room.h"
#include "Character.h"
#include "Projectile.h"

using namespace MATH;


class TeslaTowerEnemy : public PhysicsObject, public GameObject {

private:
	Matrix4 modelMatrix;
	Mesh* mesh, * meshProjectile;
	Shader* shader, * shaderProjectile;
	Texture* texture, *textureProjectile;
	Room room;
	Projectile* projectile;
	Character* character;
	Vec3 projectileDestination;
	float health = 100;
	bool isDead = false;
public:
	TeslaTowerEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_, Character* character);
	//RatEnemy(RatEnemy* parent_, Mesh* mesh_, Shader* shader_, Texture* texture_);
	~TeslaTowerEnemy();
	void BuildProjectile();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	Vec3 GenerateProjectileDirection();

	bool DamageCheck(Character* character);

	bool WeaponColCheck(Character* character);

	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }
};

#endif