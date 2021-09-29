#ifndef SCENE1_H
#define SCENE1_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Character.h"
#include "SnakeEnemy.h"
#include "Room.h"
#include "StaticMesh.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;
class DemoObject;
class Mesh;
class Shader;
class Texture;

class Scene1 : public Scene {
private:
	Character* character;
	SnakeEnemy* enemy1;
	StaticMesh* wall1, *wall2, *wall3, *wall4, *floor;
	//DemoObject* demoObject;
	Vec3 light1;
	float speed;

	Mesh* meshPtr, *ratMeshPtr, *boxMesh;
	Shader* shaderPtr;
	Texture* texturePtr, *wallTexture, *floorTexture;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Room room;
public:
	explicit Scene1();
	virtual ~Scene1();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;

	void BuildCharacter();
	void BuildSnake();
	void BuildWall();
	void BuildFloor();

	virtual float setSpeed() override;
	virtual void getSpeed(const float speed_) override;
};


#endif // SCENE0_H