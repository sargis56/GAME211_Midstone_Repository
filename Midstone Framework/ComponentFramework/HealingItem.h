#ifndef HealingItem_H
#define HealingItem_H
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


class HealingItem : public PhysicsObject, public GameObject {

private:
	Matrix4 modelMatrix;
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
	Room room;
	float healthIncrease;
	bool itemActive;
public:
	HealingItem(Mesh* mesh_, Shader* shader_, Texture* texture_, float health_, Vec3 pos_);
	//RatEnemy(RatEnemy* parent_, Mesh* mesh_, Shader* shader_, Texture* texture_);
	~HealingItem();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	float collisionCheck(Character* character, float playerHealth);

	inline bool getActive() const { return itemActive; }
	//inline float getSpeedIncrease() const { return speedIncrease; }
	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }
};

#endif