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
		enum class Scene { Title, Game, Gameover};
	public:
		// member data access
		const float& GetGravity() { return m_Gravity; }
		const float& GetDeceleration() { return m_Deceleration; }
		// special operations (template)
		void SetTarget(Surface* surface, Input* controls) { screen = surface; input = controls; }
		void Init();
		void Shutdown();
		void Tick(float dt);
		// special operations (game)
		void ChangeScene(Scene a_Scene) { m_Scene = a_Scene; screen->Clear(0); };
		void ScoreUp() { m_Score++; }
		void Gameover();
	private:
		// attributes
		const float m_Gravity = 18.0f;
		const float m_Deceleration = 0.995f;
		Scene m_Scene = Scene::Title;
		int m_HighScore = 200;
		int m_Score = 0;
		Surface* screen;
		Input* input;
	};
};