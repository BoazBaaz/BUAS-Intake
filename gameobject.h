#pragma once

#include "game.h"

namespace Tmpl8 {
	class Input;
	class Game;

	class GameObject { // base class for all other objects
	protected:
		enum class Shape { Rectangle, Circle };
	public:
		// constructor / destructor
		GameObject(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_Speed, bool a_IsRectangle = true);	// sprite, pos, vel, speed
		GameObject(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, bool a_IsRectangle = true);					// sprite, pos, vel
		GameObject(Sprite& a_Sprite, vec2 a_Position, bool a_IsRectangle = true);									// sprite, pos
		~GameObject() = default;
		// member data access
		Sprite& GetSprite() { return m_Sprite; }
		vec2 GetSpriteSize() { return m_SpriteSize; }
		void SetPosition(vec2 a_Position) { m_Position = a_Position; }
		vec2 GetPosition() { return m_Position; }
		vec2 GetCenterPosition() { return m_CenterPosition; }
		void SetVelocity(vec2 a_Velocity) { m_Velocity = a_Velocity; }
		vec2 GetVelocity() { return m_Acceleration; }
		void SetSpeed(float a_Speed) { m_Speed = a_Speed; }
		// special operations
		void Draw(Surface* screen);
	protected:
		// attributes
		Sprite& m_Sprite;
		const vec2 m_SpriteSize;
		vec2 m_Position;
		vec2 m_CenterPosition;
		vec2 m_Acceleration;
		vec2 m_Velocity;
		float m_Speed = 1.0f;
		bool m_OnScreen = false;
		const Shape m_Shape;
	};

	class UI : public GameObject {
		enum class ButtonState { None, Hover, Pressed, Held, Released};
	public:
		// constructor / destructor
		UI(Sprite& a_Sprite, vec2 a_Position, bool a_IsRectangle = true);
		~UI() = default;
		// member data access
		bool Pressed() { return m_ButtonState == ButtonState::Pressed; }
		bool Released() { return m_ButtonState == ButtonState::Released; }
		// special operations
		void Update(Surface* screen, Input* input);
	private:
		// attributes
		ButtonState m_ButtonState;
	};

	class Platform : public GameObject {
	public:
		// constructor / destructor
		Platform(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_Speed);
		~Platform() = default;
		// member data access
		void SetPlatformHit(bool a_PlatformHit) { m_PlatformHit = a_PlatformHit; }
		bool GetPlatformHit() { return m_PlatformHit; }
		// special operations
		void Update(Surface* screen, float& dt);
	private:
		// attributes
		float m_MaxRand = (ScreenWidth - m_SpriteSize.x) - 20;
		bool m_PlatformHit = false;
	};

	class Player : public GameObject {
	public:
		// constructor / destructor
		Player(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_PlayerSpeed);
		~Player() = default;
		// member data access
		bool GetGroundCollision() { return m_GroundCollision; }
		// special operations
		void Update(Game* game, Surface* screen, Input* input, float& dt);
		void BouncePhysics(Game* game, float& dt);
		void PlatformCollision(Game* game, Platform& a_CollisionObject);
	private:
		// attributes
		bool m_Boost = false;
		float m_BoostPower = 30;
		float m_BoostDropForce = 30;
		float m_GroundBuffer = 0.2f;
		bool m_GroundCollision = false;
	};
}