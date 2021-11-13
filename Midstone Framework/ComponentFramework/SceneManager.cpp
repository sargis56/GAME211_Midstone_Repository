#include <SDL.h>
#include "SceneManager.h"
#include "Timer.h"
#include "Window.h"
#include "SceneDev.h"
#include "SceneTest.h"
#include "TitleScreen.h"
#include "Scene00.h"

SceneManager::SceneManager(): 
	currentScene(nullptr), window(nullptr), timer(nullptr),
	fps(60), isRunning(false), fullScreen(false) {
}

SceneManager::~SceneManager() {
	if (currentScene) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}
	
	if (timer) {
		delete timer;
		timer = nullptr;
	}

	if (window) {
		delete window;
		window = nullptr;
	}
	Debug::Info("Deleting the GameSceneManager", __FILE__, __LINE__);

}

bool SceneManager::Initialize(std::string name_, int width_, int height_) {

	window = new Window();
	if (!window->OnCreate(name_, width_, height_)) {
		Debug::FatalError("Failed to initialize Window object", __FILE__, __LINE__);
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		Debug::FatalError("Failed to initialize Timer object", __FILE__, __LINE__);
		return false;
	}
	/********************************   Default first scene   ***********************/
	BuildScene(TITLESCREEN);
	
	return true;
}


void SceneManager::Run() {
	timer->Start();
	isRunning = true;
	while (isRunning) {
		timer->UpdateFrameTicks();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();
		GetEvents();
		SwitchScene();
		SDL_GL_SwapWindow(window->getWindow());
		SDL_Delay(timer->GetSleepTime(fps));
	}
}

void SceneManager::GetEvents() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		if (sdlEvent.type == SDL_EventType::SDL_QUIT) {
			isRunning = false;
			return;
		}
		else if (sdlEvent.type == SDL_KEYDOWN) {
			switch (sdlEvent.key.keysym.scancode) {
			case SDL_SCANCODE_F2:
				BuildScene(SCENETEST);
				break;
			case SDL_SCANCODE_RETURN:
				if (fullScreen == false) {
					fullScreen = true;
					SDL_SetWindowFullscreen(window->getWindow(), SDL_WINDOW_FULLSCREEN);
				}

			default:
				currentScene->HandleEvents(sdlEvent);
				break;
			}
		}
		if (currentScene == nullptr) {
			Debug::FatalError("Failed to initialize Scene", __FILE__, __LINE__);
			isRunning = false;
			return;
		}
		
		currentScene->HandleEvents(sdlEvent);
	}
}

void SceneManager::SwitchScene() {
	int testSceneNumber = currentScene->SetScene(); //set variable to test
	if (currentNumber != testSceneNumber) {//test to see if the scene should switch
		roomCleared[currentNumber] = currentScene->setSceneCleared();
		currentNumber = testSceneNumber;
		if (currentNumber == 1) {
			BuildScene(SCENE00);
		}
	}
}

void SceneManager::BuildScene(SCENE_NUMBER scene) {
	bool status; 
	if (currentScene != nullptr) {
		//get scene variables to be stored
		//get character variables to be stored
		mainHealth = currentScene->setCharacterHealth();
		mainPos = currentScene->setCharacterPos();
		mainSpeed = currentScene->setCharacterSpeed();
		mainWeapon = currentScene->setWeapon();
		delete currentScene;
		currentScene = nullptr;
	}

	switch (scene) {
	case TITLESCREEN:
		currentScene = new TitleScreen();
		status = currentScene->OnCreate();
		break;
	case SCENEDEV:
		currentScene = new SceneDev();
		currentScene->getSceneCleared(false);
		currentScene->getCharacterHealth(50);
		currentScene->getCharacterPos(Vec3(1,0,-15));
		currentScene->getCharacterSpeed(0.1);
		currentScene->getWeapon(0);
		status = currentScene->OnCreate();
		break;
	case SCENETEST:
		currentScene = new SceneDev();
		currentScene->getSceneCleared(false);
		currentScene->getCharacterHealth(50);
		currentScene->getCharacterPos(Vec3(1, 0, -15));
		currentScene->getCharacterSpeed(0.1);
		currentScene->getWeapon(0);
		status = currentScene->OnCreate();
		break;
	case SCENE00:
		currentScene = new Scene00();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	default:
		Debug::Error("Incorrect scene number assigned in the manager", __FILE__, __LINE__);
		currentScene = nullptr;
		break;
	}	
}


