#ifndef WEAPON_H
#define WEAPON_H
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

class Weapon : public PhysicsObject, public GameObject {

//private:
protected:
	Matrix4 modelMatrix;
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
	Room room;
//protected:
	//Used for scene transferring
	int weaponID = 0;
	bool weaponDropActive = false;
	float swingDamage = 0;
	float stabDamage = 0;
	float reach = 0;
	float delay = 0;
	bool altMode = false;
	float altSwingDamage = 0;
	float altStabDamage = 0;
	float altReach = 0;
	float altDelay = 0;
	// 1 = Chop | 2 = Cut | 3 = Blunt
	int damageType = 0;
	int altDamageType = 2;
public:
	Weapon();
	Weapon(Mesh* mesh_, Shader* shader_, Texture* texture_, Vec3 pos_);
	~Weapon();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime_) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	bool collisionCheck(Character* character);

	inline bool getActive() const { return weaponDropActive; }
	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }

	//Weapon getters
	inline float getWeaponID() const { return weaponID; }
	inline float getDamageType() const { return damageType; }
	inline float getAltDamageType() const { return altDamageType; }
	inline float getSwingDamage() const { return swingDamage; }
	inline float getAltSwingDamage() const { return altSwingDamage; }
	inline float getStabDamage() const { return stabDamage; }
	inline float getAltStabDamage() const { return altStabDamage; }
	inline float getReach() const { return reach; }
	inline float getAltReach() const { return altReach; }
	inline float getDelay() const { return delay; }
	inline float getAltDelay() const { return altDelay; }
};

#endif