#pragma once

#include "surface.h"
#include "template.h"
#include "input.h"
#include "SDL_Scancode.h"
#include <iostream>
#include <string>

namespace Tmpl8 {
	class Surface;
	class Input;

	class Game {
		enum class Scene { Main, Game };
		Surface* screen;
		Input* input;
	public:
		// member data access
		const float& GetGravity() { return m_Gravity; }
		const float& GetDeceleration() { return m_Deceleration; }
		// special operations
		void SetTarget(Surface* surface, Input* controls) { screen = surface; input = controls; }
		void Init();
		void Shutdown();
		void Tick(float& dt);
		bool Button(GameObject sObj);
		void ChangeScene(Scene a_Scene) { m_Scene = a_Scene; screen->Clear(0); };
	private:
		// attributes
		const float m_Gravity = 9.81;
		const float m_Deceleration = 0.995;
		Scene m_Scene = Scene::Main;
	};
};