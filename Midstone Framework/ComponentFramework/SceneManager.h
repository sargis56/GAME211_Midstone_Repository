#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <string>
#include "Vector.h"
using namespace MATH;
class SceneManager  {
public:
	
	SceneManager();
	~SceneManager();
	void Run();
	bool Initialize(std::string name_, int width_, int height_);
	void GetEvents();
	void SwitchScene();
	
private:
	enum SCENE_NUMBER {
		SCENETEST = 0,
		TITLESCREEN,
		DEATHSCREEN,
		WINSCREEN,
		SCENEDEV,
		SCENE00,
		SCENE00UNDER,
		SCENE10,
		SCENE10UNDER,
		SCENE0NEG1,
		SCENE1NEG1,
		SCENE0NEG2,
		SCENE01,
		SCENE11,
		SCENE12,
		SCENE20,
		SCENE30,
		SCENE0NEG1UNDER,
		SCENE1NEG1UNDER,
		SCENE0NEG2UNDER,
		SCENE01UNDER,
		SCENE11UNDER,
		SCENE12UNDER,
		SCENE20UNDER,
		SCENE30UNDER
	};

	class Scene* currentScene;
	class Timer* timer;
	class Window* window;

	unsigned int fps;
	int currentNumber = 0; //currentScene Number
	bool isRunning;
	bool fullScreen;
	void BuildScene(SCENE_NUMBER scene_);
	//scene variables
	bool roomCleared[40] = { false, false, false, false, false, false, false, false, false, false, false, false, false, 
		false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }; //array should have # of rooms in the game
	//characterStats
	float mainHealth;
	Vec3 mainPos;
	float mainSpeed;
	int mainWeapon;
	bool playerDimension = false;
};


#endif // SCENEMANAGER_H