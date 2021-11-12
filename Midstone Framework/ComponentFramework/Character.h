#ifndef CHARACTER_H
#define CHARACTER_H
#include <glew.h>
#include "Matrix.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "PhysicsObject.h"
#include "Room.h"

using namespace MATH;


class Character : public PhysicsObject, public GameObject {

private:
	Matrix4 modelMatrix;
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
	Room room;
	bool keyR = false, keyL = false, keyUp = false, keyDown = false;
	float speed = 0.1;
	bool invincibility = false;
	bool visibility = true;
	int invincibilityTimer;

public:
	Character(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_);
	//Character(Character* parent_, Mesh* mesh_, Shader* shader_, Texture* texture_);
	~Character();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	void checkInvincibility();

	inline bool getInvincibility() const { return invincibility; }
	inline bool getVisibility() const { return visibility; }
	inline void setinvincibilityTimer(const float setinvincibilityTimer_) { invincibilityTimer = setinvincibilityTimer_; }

	inline Mesh* getMesh() const { return mesh; }
	inline Texture* getTexture() const { return texture; }
	inline void setSpeed(const float setSpeed_) { speed = setSpeed_; }
	inline float getSpeed() { return speed; }
	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }
};

#endif