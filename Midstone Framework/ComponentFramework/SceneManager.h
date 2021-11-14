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
		SCENEDEV,
		SCENE00,
		SCENE10
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
	bool roomCleared[10] = { false, false, false, false, false, false, false, false, false, false }; //array should have # of rooms in the game
	//characterStats
	float mainHealth;
	Vec3 mainPos;
	float mainSpeed;
	int mainWeapon;
};


#endif // SCENEMANAGER_H