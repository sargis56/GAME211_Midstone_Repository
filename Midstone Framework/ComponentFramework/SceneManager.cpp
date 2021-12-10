#include <SDL.h>
#include "SceneManager.h"
#include "Timer.h"
#include "Window.h"
#include "SceneDev.h"
#include "SceneTest.h"
#include "TitleScreen.h"
#include "DeathScreen.h"
#include "WinScreen.h"

#include "Scene00.h"
#include "Scene10.h"
#include "Scene0NEG1.h"
#include "Scene0NEG2.h"
#include "Scene1NEG1.h"
#include "Scene01.h"
#include "Scene11.h"
#include "Scene12.h"
#include "Scene20.h"
#include "Scene30.h"

#include "Scene00Under.h"
#include "Scene10Under.h"
#include "Scene0NEG1Under.h"
#include "Scene0NEG2Under.h"
#include "Scene1NEG1Under.h"
#include "Scene01Under.h"
#include "Scene11Under.h"
#include "Scene12Under.h"
#include "Scene20Under.h"
#include "SceneNEG11Under.h"

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
	//BuildScene(SCENETEST);

	return true;
}


void SceneManager::Run() {
	timer->Start();
	isRunning = true;
	while (isRunning) {
		timer->UpdateFrameTicks();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();
		SwitchScene();
		GetEvents();
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
			case SDL_SCANCODE_SPACE:
				if (playerDimension == false) {
					if (currentNumber == 1) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 11;
						BuildScene(SCENE00UNDER);
						playerDimension = true;
						//break;
					}
					if (currentNumber == 2) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 12;
						BuildScene(SCENE10UNDER);
						playerDimension = true;
						//break;
					}
					if (currentNumber == 3) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 13;
						BuildScene(SCENE0NEG1UNDER);
						playerDimension = true;
						//break;
					}
					if (currentNumber == 4) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 14;
						BuildScene(SCENE1NEG1UNDER);
						playerDimension = true;
						//break;
					}
					if (currentNumber == 5) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 15;
						BuildScene(SCENE0NEG2UNDER);
						playerDimension = true;
						//break;
					}
					if (currentNumber == 6) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 16;
						BuildScene(SCENE01UNDER);
						playerDimension = true;
						//break;
					}
					if (currentNumber == 7) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 17;
						BuildScene(SCENE11UNDER);
						playerDimension = true;
						//break;
					}
					if (currentNumber == 8) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 18;
						BuildScene(SCENE12UNDER);
						playerDimension = true;
						//break;
					}
					if (currentNumber == 9) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 19;
						BuildScene(SCENE20UNDER);
						playerDimension = true;
						//break;
					}
				}
				else if (playerDimension == true) {
					if (currentNumber == 11) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 1;
						BuildScene(SCENE00);
						playerDimension = false;
						//break;
					}
					if (currentNumber == 12) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 2;
						BuildScene(SCENE10);
						playerDimension = false;
						//break;
					}
					if (currentNumber == 13) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 3;
						BuildScene(SCENE0NEG1);
						playerDimension = false;
						//break;
					}
					if (currentNumber == 14) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 4;
						BuildScene(SCENE1NEG1);
						playerDimension = false;
						//break;
					}
					if (currentNumber == 15) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 5;
						BuildScene(SCENE0NEG2);
						playerDimension = false;
						//break;
					}
					if (currentNumber == 16) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 6;
						BuildScene(SCENE01);
						playerDimension = false;
						//break;
					}
					if (currentNumber == 17) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 7;
						BuildScene(SCENE11);
						playerDimension = false;
						//break;
					}
					if (currentNumber == 18) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 8;
						BuildScene(SCENE12);
						playerDimension = false;
						//break;
					}
					if (currentNumber == 19) {
						roomCleared[currentNumber] = currentScene->setSceneCleared();
						currentNumber = 9;
						BuildScene(SCENE20);
						playerDimension = false;
						//break;
					}
				}
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
		if (currentNumber == 0) {
			BuildScene(TITLESCREEN);
		}
		if (currentNumber == 1) {
			BuildScene(SCENE00);
		}
		if (currentNumber == 2) {
			BuildScene(SCENE10);
		}
		if (currentNumber == 3) {
			BuildScene(SCENE0NEG1);
		}
		if (currentNumber == 4) {
			BuildScene(SCENE1NEG1);
		}
		if (currentNumber == 5) {
			BuildScene(SCENE0NEG2);
		}
		if (currentNumber == 6) {
			BuildScene(SCENE01);
		}
		if (currentNumber == 7) {
			BuildScene(SCENE11);
		}
		if (currentNumber == 8) {
			BuildScene(SCENE12);
		}
		if (currentNumber == 9) {
			BuildScene(SCENE20);
		}

		if (currentNumber == 11) {
			BuildScene(SCENE00UNDER);
		}
		if (currentNumber == 12) {
			BuildScene(SCENE10UNDER);
		}
		if (currentNumber == 13) {
			BuildScene(SCENE0NEG1UNDER);
		}
		if (currentNumber == 14) {
			BuildScene(SCENE1NEG1UNDER);
		}
		if (currentNumber == 15) {
			BuildScene(SCENE0NEG2UNDER);
		}
		if (currentNumber == 16) {
			BuildScene(SCENE01UNDER);
		}
		if (currentNumber == 17) {
			BuildScene(SCENE11UNDER);
		}
		if (currentNumber == 18) {
			BuildScene(SCENE12UNDER);
		}
		if (currentNumber == 19) {
			BuildScene(SCENE20UNDER);
		}

		if (currentNumber == 20) {
			BuildScene(SCENE30);
		}

		if (currentNumber == 30) {
			BuildScene(WINSCREEN);
		}
		if (currentNumber == 31) {
			BuildScene(DEATHSCREEN);
		}
		if (currentNumber == 32) {
			isRunning = false;
		}

		if (currentNumber == 33) {
			BuildScene(SCENENEG11UNDER);
		}
	}
}

void SceneManager::BuildScene(SCENE_NUMBER scene) {
	bool status; 
	if (currentScene != nullptr) {
		//get scene variables to be stored
		//get character variables to be stored
		//roomCleared[currentNumber] = currentScene->setSceneCleared();
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
	case DEATHSCREEN:
		currentScene = new DeathScreen();
		status = currentScene->OnCreate();
		for (int x = 0; x < 40; x++) { //reset all rooms
			roomCleared[x] = false;
		}
		playerDimension = false;
		break;
	case WINSCREEN:
		currentScene = new WinScreen();
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
		currentScene = new SceneTest();
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
	case SCENE10:
		currentScene = new Scene10();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE10UNDER:
		currentScene = new Scene10Under();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE0NEG1:
		currentScene = new Scene0NEG1();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE0NEG2:
		currentScene = new Scene0NEG2();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE1NEG1:
		currentScene = new Scene1NEG1();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE01:
		currentScene = new Scene01();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE11:
		currentScene = new Scene11();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE12:
		currentScene = new Scene12();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE20:
		currentScene = new Scene20();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE30:
		currentScene = new Scene30();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	//Underworld cases ---------------------------------------------------------
	case SCENE00UNDER:
		currentScene = new Scene00Under();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE0NEG1UNDER:
		currentScene = new Scene0NEG1Under();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE0NEG2UNDER:
		currentScene = new Scene0NEG2Under();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE1NEG1UNDER:
		currentScene = new Scene1NEG1Under();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE01UNDER:
		currentScene = new Scene01Under();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE11UNDER:
		currentScene = new Scene11Under();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE12UNDER:
		currentScene = new Scene12Under();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENE20UNDER:
		currentScene = new Scene20Under();
		//sending the speed back to the new scene
		currentScene->getSceneCleared(roomCleared[currentNumber]);
		currentScene->getCharacterHealth(mainHealth);
		currentScene->getCharacterPos(Vec3(mainPos));
		currentScene->getCharacterSpeed(mainSpeed);
		currentScene->getWeapon(mainWeapon);
		status = currentScene->OnCreate();
		break;
	case SCENENEG11UNDER:
		currentScene = new SceneNEG11Under();
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


