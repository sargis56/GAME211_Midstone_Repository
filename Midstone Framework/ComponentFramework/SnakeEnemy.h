#ifndef SnakeEnemy_H
#define SnakeEnemy_H
#include <glew.h>
#include "Matrix.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "PhysicsObject.h"
#include "Room.h"
#include "Character.h"
#include "VenomProjectile.h"

using namespace MATH;


class SnakeEnemy : public PhysicsObject, public GameObject {

private:
	Matrix4 modelMatrix;
	Mesh* mesh, *meshVProjectile;
	Shader* shader, *shaderVProjectile;
	Texture* texture, *textureVProjectile;
	Vec3 attackTarget;
	Vec3 direction,desiredPos;
	Room room;
	float moveSpeed;
	float agroRange = 6.0f;
	int damage = 1;
	VenomProjectile *vProjectile;
	float health = 100;
	bool isDead = true;
public:
	SnakeEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_);
	//RatEnemy(RatEnemy* parent_, Mesh* mesh_, Shader* shader_, Texture* texture_);
	~SnakeEnemy();
	//void BuildVProjectile();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	bool DamageCheck(Character* character);
	float FollowPlayer(Character* character);
	void PatrolArea();
	void MoveEnemy();

	void AttackPlayer(Character* chtr);

	bool WeaponColCheck(Character* character);

	void TakeDamage(float damage);

	inline bool isAlive() const { return isDead; }
	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }
};

#endif