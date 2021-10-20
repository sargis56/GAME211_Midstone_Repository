#ifndef Projectile_H
#define Projectile_H
#include <glew.h>
#include "Matrix.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "PhysicsObject.h"
#include "Room.h"
#include "Character.h"

#define LOG(x) std::cout << x << std::endl;

using namespace MATH;


class Projectile : public PhysicsObject, public GameObject {

private:
	Matrix4 modelMatrix;
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
	bool moveOver = true;
	float direction;
	float damage;
	float timeBtwnShots;
	Room room;
public:
	Projectile(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_, Vec3 pos, int damage);
	//RatEnemy(RatEnemy* parent_, Mesh* mesh_, Shader* shader_, Texture* texture_);
	~Projectile();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	inline void setOver(const bool& over_) { moveOver = over_; }

	bool ProjectileUpdate4Axis(Vec3 direction);
	bool ProjectileUpdate8Axis(Vec3 direction);
	bool DamageCheck(Character* character);

	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }
};

#endif