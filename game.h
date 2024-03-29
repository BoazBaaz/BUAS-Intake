#pragma once

#include "surface.h"
#include "template.h"
#include "input.h"
#include "gameobject.h"
#include "SDL_Scancode.h"
#include <algorithm>
#include <iostream>
#include <fstream>

namespace Tmpl8 {
	class Surface;
	class Input;

	class Game {
		enum class Scene { Title, Game, Gameover };
	public:
		// member data access
		const float& GetGravity() { return gravity; }
		const float& GetDeceleration() { return deceleration; }
		float GetPlatformSpeed() { return platformSpeed; }
		int GetScore() { return curScore; }
		// special operations (template)
		void SetTarget(Surface* surface, Input* controls) { screen = surface; input = controls; }
		void Init();
		void Shutdown();
		int Tick(float dt);
		// special operations (game)
		void ChangeScene(Scene newScene) { curScene = newScene; screen->Clear(0); };
		void DrawScore(Sprite& sprite, vec2 position, int score);
		void DrawDigits(vec2 position, int value);
		void SpeedUp() { platformSpeed += platformSpeedIncrease / 100 * platformSpeed; }
		void ScoreUp() { curScore++; }
		void StartGame();
		void GameOver();
	private:
		// attributes
		const float gravity = 18.0f;
		const float deceleration = 0.995f;
		Scene curScene = Scene::Title;
		int highScore = 0;
		int curScore = 0;
		float platformSpeed = 40.0f;
		float platformSpeedIncrease = 1.410148f; // the exponential increase percentage (at max 99 the speed is 160.0000904821)
		Surface* screen;
		Input* input;
	};
};