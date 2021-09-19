#include "Scene0.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Randomizer.h"
#include <iostream>

Scene0::Scene0(SDL_Window* sdlWindow_) {
	Randomizer r;
	window = sdlWindow_;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	for (int i = 0; i < NUM_BALLS; ++i) {

		ballArray[i] = new Character(Vec3(0.0f, 2.0f, 0.0f),
			Vec3(0.0, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	}
}

Scene0::~Scene0() {
	for (int i = 0; i < NUM_BALLS; ++i) {
		delete ballArray[i];
	}
	SDL_DestroyRenderer(renderer);
}

bool Scene0::OnCreate() {
	int w, h;
	float xAxis = 30.0f;
	float yAxis = 15.0f;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* ballImage = IMG_Load("textures/ball.png");
	SDL_Texture* ballTexture = SDL_CreateTextureFromSurface(renderer, ballImage);
	if (ballTexture == nullptr) printf("%s\n", SDL_GetError());
	if (ballImage == nullptr) {
		std::cerr << "Can't open the image" << std::endl;
		return false;
	}
	else {

		Vec3 worldCoordsFromScreenCoords;
		worldCoordsFromScreenCoords.x = xAxis / static_cast<float>(w) * static_cast<float>(ballImage->w);
		worldCoordsFromScreenCoords.y = yAxis / static_cast<float>(h) * static_cast<float>(ballImage->h);
		worldCoordsFromScreenCoords.z = 0.0f;

		for (int i = 0; i < NUM_BALLS; i++) {
			ballArray[i]->setTexture(ballTexture);
			ballArray[i]->setImageSizeWorldCoords(worldCoordsFromScreenCoords);
		}
		SDL_FreeSurface(ballImage);

	}
	return true;
}

void Scene0::OnDestroy() {}

void Scene0::Update(const float deltaTime) {
	for (int i = 0; i < NUM_BALLS; i = i + 1) {
		ballArray[i]->Update(deltaTime);
	}
}


void Scene0::HandleEvents(const SDL_Event& sdlEvent) {
	for (int i = 0; i < NUM_BALLS; i++) {
		ballArray[i]->HandleEvents(sdlEvent);
	}
}


void Scene0::Render() const {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_Rect square;
	Vec3 screenCoords;
	int w, h;
	static double rot = 0.0;
	for (int i = 0; i < NUM_BALLS; ++i) {
		SDL_QueryTexture(ballArray[i]->getTexture(), nullptr, nullptr, &w, &h);
		screenCoords = projectionMatrix * ballArray[i]->getPos();
		square.x = static_cast<int>(screenCoords.x);
		square.y = static_cast<int>(screenCoords.y);
		square.w = w;
		square.h = h;
		SDL_RenderCopyEx(renderer, ballArray[i]->getTexture(), nullptr, &square, rot, nullptr, SDL_FLIP_NONE);
		rot += 10.0;
	}
	SDL_RenderPresent(renderer);
}