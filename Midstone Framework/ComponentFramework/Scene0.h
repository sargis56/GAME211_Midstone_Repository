#ifndef SCENE0_H
#define SCENE0_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Character.h"
#include "RatEnemy.h"
#include "Room.h"
#include "StaticMesh.h"
#include "MageTurretEnemy.h"
#include "TeslaTowerEnemy.h"
#include "Door.h"
#include "HealthUI.h"

using namespace MATH;

/// Forward declarations 
union SDL_Event;
class DemoObject;
class Mesh;
class Shader;
class Texture;

class Scene0 : public Scene {
private:
	Character* character;
	Door* doorLeft;
	TeslaTowerEnemy* enemy1;
	StaticMesh* wall1, *wall2, *wall3, *wall4, *floor;
	HealthUI* healthBar;
	//DemoObject* demoObject;
	Vec3 light1;
	float health;

	Mesh* meshPtr, *ratMeshPtr, *boxMesh;
	Shader* shaderPtr;
	Texture* texturePtr, *wallTexture, *floorTexture, *turretTexture, *doorTexture, *healthUITexture;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Room room;

	int sceneNumber = 1;
public:
	explicit Scene0();
	virtual ~Scene0();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
	virtual int SetScene() override;

	void BuildCharacter();
	void BuildRat();
	void BuildWall();
	void BuildFloor();
	void BuildDoor();

	void BuildHealthUI();

	virtual float setCharacterVariables() override;
	virtual void getCharacterVariables(const float stats_) override;
};


#endif // SCENE0_H