#ifndef ProjectileDynamic_H
#define ProjectileDynamic_H
#include <glew.h>
#include "Matrix.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "PhysicsObject.h"
#include "Room.h"
#include "Character.h"

//#define LOG(x) std::cout << x << std::endl; //debugging function

using namespace MATH;


class ProjectileDynamic : public PhysicsObject, public GameObject {

private:
	Matrix4 modelMatrix;
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
	bool moveOver = true;
	float direction;
	float damage;
	float radius;
	Room room;
public:
	ProjectileDynamic(Mesh* mesh_, Shader* shader_, Texture* texture_, Room room_, Vec3 pos, int damage, float radius);
	//RatEnemy(RatEnemy* parent_, Mesh* mesh_, Shader* shader_, Texture* texture_);
	~ProjectileDynamic();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	inline void setOver(const bool& over_) { moveOver = over_; }

	bool ProjectileDynamicUpdate(Vec3 direction, float deltaTime);
	bool DamageCheck(Character* character);

	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }
};

#endif