#ifndef MageEnemy_H
#define MageEnemy_H
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

using namespace MATH;


class MageEnemy : public PhysicsObject, public GameObject {

private:
	Matrix4 modelMatrix;
	Mesh* mesh, * meshProjectile;
	Shader* shader, * shaderProjectile;
	Texture* texture, *textureProjectile;
	Room room;
	ProjectileDynamic* projectile;
	Character* character;
	float health = 500;
	bool isDead = true;
	int textureChangeTimer = 0;
	bool textureChange = false;
	float angle;
public:
	MageEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_, Character* character);
	//RatEnemy(RatEnemy* parent_, Mesh* mesh_, Shader* shader_, Texture* texture_);
	~MageEnemy();
	void BuildProjectile();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	bool WeaponColCheck(Character* character);
	void TakeDamage(float damage);
	virtual void HandleEvents(const SDL_Event& event) override;

	bool DamageCheck(Character* character);
	inline bool isAlive() const { return isDead; }
	inline int getTimer() const { return textureChangeTimer; }
	void ResetTimer();
	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }
	inline float getRotation() const { return angle; }
};

#endif