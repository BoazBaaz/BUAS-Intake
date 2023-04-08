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
	class GameObject;

	class Game {
		enum class Scene { Title, Game, Gameover };
	public:
		// member data access
		const float& GetGravity() { return gravity; }
		const float& GetDeceleration() { return deceleration; }
		// special operations (template)
		void SetTarget(Surface* surface, Input* controls) { screen = surface; input = controls; }
		void Init();
		void Shutdown();
		int Tick(float dt);
		// special operations (game)
		void ChangeScene(Scene newScene) { curScene = newScene; screen->Clear(0); };
		void ScoreUp() { curScore++; }
		void DrawScore(Sprite& sprite, vec2 position, int score);
		void DrawDigits(vec2 position, int value);
		void StartGame();
		void GameOver();
	private:
		// attributes
		const float gravity = 18.0f;
		const float deceleration = 0.995f;
		Scene curScene = Scene::Title;
		int highScore = 0;
		int curScore = 0;
		Surface* screen;
		Input* input;
	};
};