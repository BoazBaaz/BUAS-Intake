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
	protected:
		enum class ObjectType { Static, Dynamic }; // I made it a enum so I could add more types later... maybe
	public:
		// constructor / destructor
		GameObject(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, bool a_IsDynamic = false);
		GameObject(Sprite& a_Sprite, vec2 a_Position);
		~GameObject() = default;
		// member data access
		Sprite& GetSprite() { return m_Sprite; }
		vec2& GetPosition() { return m_Position; }
		vec2& GetVelocity() { return m_Velocity; }
		void SetSpeed(float a_Speed) { m_Speed = a_Speed; }
		// special operations
		virtual void Update(Surface* screen, float& dt, const float& gravity, const float& deceleration);
	protected:
		// attributes
		Sprite& m_Sprite;
		vec2 m_Position;
		vec2 m_Velocity;
		float m_Speed = 1;
		ObjectType m_ObjectType;
	};

	class Player : public GameObject {
	public:
		// constructor / destructor
		Player(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_PlayerSpeed);
		// member data access
		// special operations
		void Update(Surface* screen, float& dt, const float& gravity, const float& deceleration);
	private:
		// attributes
		bool m_GroundHit = false;
	};

	class Game {
		enum class Scene { Main, Game };
		Surface* screen;
		Input* input;
	public:
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