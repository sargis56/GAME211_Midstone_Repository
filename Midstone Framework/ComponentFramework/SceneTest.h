#ifndef SCENETEST_H
#define SCENETEST_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "Character.h"
#include "RatEnemy.h"
#include "SnakeEnemy.h"
#include "Room.h"
#include "StaticMesh.h"
#include "MageTurretEnemy.h"
#include "MageEnemy.h"
#include "ArcherEnemy.h"
#include "SpeedItem.h"
#include "Door.h"
#include "HealthUI.h"
#include "HealingItem.h"
#include "Weapon.h"
#include "Shovel.h"
#include "Axe.h"
#include "Sword.h"
#include "Zweihander.h"

using namespace MATH;

/// Forward declarations 
union SDL_Event;
class DemoObject;
class Mesh;
class Shader;
class Texture;

class SceneTest : public Scene {
private:
	//character pointers and variables
	Character* character;
	Mesh* meshPtr, * ratMeshPtr, * boxMesh, *itemMesh, *snakeMeshPtr, *swordMesh, *shovelMesh, *axeMesh, *zweiMesh;
	float health, speed;
	int weapon;
	Vec3 returnedPos;
	//room setup pointers
	Room room;
	StaticMesh* wall1, * wall2, * wall3, * wall4, * floor;
	HealthUI* healthBar;
	Vec3 light1;
	bool roomCleared = false;
	bool roomUpdate = false;

	//Enemies and Items
	ArcherEnemy* enemy1;
	SpeedItem* speedItem;
	HealingItem* healingItem;
	SnakeEnemy* snakeEnemy;
	Shovel* shovel;
	Axe* axe;
	Sword* sword;
	Zweihander* zwei;

	//Shader Pointer
	Shader* shaderPtr;

	//Texture Pointer
	Texture* texturePtr, *wallTexture, *floorTexture, *turretTexture, *doorTexture, *healthUITexture, *snakeTexture;

	//Engine Variables
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	int sceneNumber = -1;
public:
	//Scene setup
	explicit SceneTest();
	virtual ~SceneTest();
	//Scene inherited functions
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
	virtual int SetScene() override;
	//Variable Passing
	virtual bool setSceneCleared() override;
	virtual void getSceneCleared(const bool storedScene_) override;
	virtual float setCharacterHealth() override;
	virtual void getCharacterHealth(const float storedHealth_) override;
	virtual Vec3 setCharacterPos() override;
	virtual void getCharacterPos(const Vec3 storedPos_) override;
	virtual float setCharacterSpeed() override;
	virtual void getCharacterSpeed(const float storedSpeed_) override;
	virtual int setWeapon() override;
	virtual void getWeapon(const int storedWeapon_) override;
	//Gameobjects creation functions
	void BuildCharacter();
	void BuildAllEnemies();
	void BuildRoom();
	//UI creation functions
	void BuildHealthUI();
};


#endif