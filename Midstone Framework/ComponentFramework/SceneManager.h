#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "CharacterVariables.h"
#include <string>
class SceneManager  {
public:
	
	SceneManager();
	~SceneManager();
	void Run();
	bool Initialize(std::string name_, int width_, int height_);
	void GetEvents();
	static void CurrentScene(std::string scene);
	
private:
	enum SCENE_NUMBER {
		SCENE0 = 0,
		TITLESCREEN,
		SCENE1,
		SCENE3,
		SCENE4,
		SCENE5,
		SCENE6,
		SCENEPHYSICS,
		SCENEPHYSICS2,
		SCENEPHYSICS3,
		SCENEPHYSICS4
	};

	class Scene* currentScene;
	class Timer* timer;
	class Window* window;

	unsigned int fps;
	bool isRunning;
	bool fullScreen;
	void BuildScene(SCENE_NUMBER scene_);

	//characterStats
	CharacterVariables mainStats;
};


#endif // SCENEMANAGER_H