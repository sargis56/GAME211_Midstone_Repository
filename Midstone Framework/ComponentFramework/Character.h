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
	bool isDead = false;
	int damage = 0;
	//float mx, my;
	//float twoPointAngle;
	float rotation;
	bool swing = true;
	bool stab = false;
	bool attacking = false;

	//Player's weapon stats
	int currentWeaponID = 0;
	float currentSwingDamage;
	float currentStabDamage;
	float currentWeaponReach;
	float currentWeaponDelay;
	
	float swingDamage;
	float stabDamage;
	float weaponReach;
	float weaponDelay;

	bool altWeaponMode;
	float altSwingDamage;
	float altStabDamage;
	float altWeaponReach;
	float altWeaponDelay;
	// 1 = Chop | 2 = Cut | 3 = Blunt
	int weaponDamageType;
	int altWeaponDamageType;
	int currentDamageType;

	float damageFromPlayer;

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

	//bool onDeath();

	inline Mesh* getMesh() const { return mesh; }
	inline Texture* getTexture() const { return texture; }
	inline void setSpeed(const float setSpeed_) { speed = setSpeed_; }
	inline float getSpeed() { return speed; }
	inline Shader* getShader() const { return shader; }
	inline void setModelMatrix(const Matrix4& modelMatrix_) { modelMatrix = modelMatrix_; }
	inline const Matrix4& getModelMatrix() { return modelMatrix; }

	//inline float getTwoPointAngle() const { return twoPointAngle; }
	//inline float getPlayerMouseY() const { return my; }
	//inline float getPlayerMouseX() const { return mx; }
	inline float getRotation() const { return rotation; }
	inline bool getAttacking() const { return attacking; }

	//Player's weapon getters
	inline int getCurrentWeaponID() const { return currentWeaponID; }
	inline int getCurrentDamageType() const { return currentDamageType; }
	inline float getCurrentSwingDamage() const { return currentSwingDamage; }
	inline float getCurrentStabDamage() const { return currentStabDamage; }
	inline float getCurrentWeaponDelay() const { return currentWeaponDelay; }
	inline float getCurrentWeaponReach() const { return currentWeaponReach; }

	inline float getDamageFromPlayer() const { return currentWeaponReach; }

	//Player's weapon setters
	inline void setCurrentWeaponID(const int setCurrentWeaponID_) { currentWeaponID = setCurrentWeaponID_; }

	inline void setWeaponDamageType(const int setWeaponDamageType_) { weaponDamageType = setWeaponDamageType_; }
	inline void setAltWeaponDamageType(const int setAltWeaponDamageType_) { altWeaponDamageType = setAltWeaponDamageType_; }
	inline void setCurrentDamageType(const int setCurrentDamageType_) { currentDamageType = setCurrentDamageType_; }

	inline void setAltWeaponMode(const bool setAltWeaponMode_) { altWeaponMode = setAltWeaponMode_; }
	
	inline void setSwingDamage(const float setSwingDamage_) { swingDamage = setSwingDamage_; }
	inline void setAltSwingDamage(const float setAltSwingDamage_) { altSwingDamage = setAltSwingDamage_; }
	inline void setCurrentSwingDamage(const float setCurrentSwingDamage_) { currentSwingDamage = setCurrentSwingDamage_; }

	inline void setStabDamage(const float setStabDamage_) { stabDamage = setStabDamage_; }
	inline void setAltStabDamage(const float setAltStabDamage_) { altStabDamage = setAltStabDamage_; }
	inline void setCurrentStabDamage(const float setCurrentStabDamage_) { currentStabDamage = setCurrentStabDamage_; }

	inline void setWeaponReach(const float setWeaponReach_) { weaponReach = setWeaponReach_; }
	inline void setAltWeaponReach(const float setAltWeaponReach_) { altWeaponReach = setAltWeaponReach_; }
	inline void setCurrentWeaponReach(const float setCurrentWeaponReach_) { currentWeaponReach = setCurrentWeaponReach_; }

	inline void setWeaponDelay(const float setWeaponDelay_) { weaponDelay = setWeaponDelay_; }
	inline void setAltWeaponDelay(const float setAltWeaponDelay_) { altWeaponDelay = setAltWeaponDelay_; }
	inline void setCurrentWeaponDelay(const float setCurrentWeaponDelay_) { currentWeaponDelay = setCurrentWeaponDelay_; }
};

#endif