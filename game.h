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

	class GameObject {
		enum class ObjectType { Static, Dynamic };
	public:
		// constructor / destructor
		GameObject(Sprite* a_Sprite, vec2 a_Position, vec2 a_Velocity, bool a_IsDynamic = false);
		GameObject(Sprite* a_Sprite, vec2 a_Position);
		~GameObject();
		// member data access
		Sprite* GetSprite() { return m_Sprite; }
		vec2& GetPosition() { return m_Position; }
		void SetPosition(vec2 a_Position) { m_Position = a_Position; }
		vec2& GetVelocity() { return m_Velocity; }
		void SetVelocity(vec2 a_Velocity) { m_Velocity = a_Velocity; }
		// special operations
	private:
		// attributes
		Sprite* m_Sprite;
		vec2 m_Position;
		vec2 m_Velocity;
		ObjectType m_ObjectType;
	};

	class Game {
	public:
		void SetTarget(Surface* surface, Input* controls) { screen = surface; input = controls; }
		void Init();
		void Shutdown();
		void Tick(float dt);

	private:
		Surface* screen;
		Input* input;

		enum Scene { main, game } curScene = main;

		const double gravity = 9.81;
		const double deceleration = 0.995;

		bool Button(GameObject sObj);
		void Physics(GameObject& p, float dt);
		void Collision(GameObject& p);
		void ChangeScene(Scene newScene) { curScene = newScene; screen->Clear(0); };
	};
};