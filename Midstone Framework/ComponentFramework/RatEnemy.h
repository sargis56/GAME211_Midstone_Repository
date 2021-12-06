#ifndef RatEnemy_H
#define RatEnemy_H
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


class RatEnemy : public PhysicsObject, public GameObject {

private:
	Matrix4 modelMatrix;
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
	bool moveOver = true;
	float direction;
	Vec3 desiredPos;
	Room room;
	float moveSpeed;
	float health = 50;
	bool isDead = true;
	float angle;
	int textureChangeTimer = 0;
	bool textureChange = false;
public:
	RatEnemy(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_);
	//RatEnemy(RatEnemy* parent_, Mesh* mesh_, Shader* shader_, Texture* texture_);
	~RatEnemy();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	bool DamageCheck(Character* character);
	bool WeaponColCheck(Character* character);
	void TakeDamage(float damage);
	//bool WeaponColCheck(Character* character);
	inline bool isAlive() const { return isDead; }
	inline int getTimer() const { return textureChangeTimer; }
	void ResetTimer();
	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }
	inline float getRotation() const { return angle; }
};

#endif