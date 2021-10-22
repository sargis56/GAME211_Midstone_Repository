#ifndef SCENE1_H
#define SCENE1_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Character.h"
#include "SnakeEnemy.h"
#include "DemonEnemy.h"
#include "Room.h"
#include "Door.h"
#include "StaticMesh.h"
#include "HealthUI.h"
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
	Door* doorRight;
	SnakeEnemy* enemy1;
	DemonEnemy* enemyDemon0;
	StaticMesh* wall1, *wall2, *wall3, *wall4, *floor;
	//HealthUI* healthBar;
	//DemoObject* demoObject;
	Vec3 light1;
	float health;

	Mesh* meshPtr, *ratMeshPtr, * demonMeshPtr, *boxMesh;
	Shader* shaderPtr;
	Texture* texturePtr, * wallTexture, * floorTexture, * ratTexture, * demonTexture, * doorTexture, * healthUITexture;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Room room;

	int sceneNumber = 2;
public:
	explicit Scene1();
	virtual ~Scene1();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
	virtual int SetScene() override;

	void BuildCharacter();
	void BuildSnake();
	void BuildDemon();
	void BuildWall();
	void BuildFloor();
	void BuildDoor();

	void BuildHealthUI();

	virtual float setCharacterVariables() override;
	virtual void getCharacterVariables(const float stats_) override;
};


#endif // SCENE0_H