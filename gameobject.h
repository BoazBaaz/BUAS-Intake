#pragma once

#include "game.h"

namespace Tmpl8 {
	class Game;
	class Input;
	class Player;

	class GameObject {
	protected:
		enum class ObjectType { Static, Dynamic, UI }; 
		enum class Shape { Rectangle, Circle };
		struct Collision {
			bool x;
			bool y;
		};
	public:
		// constructor / destructor
		GameObject(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_Speed, bool a_IsRectangle = true, bool a_IsDynamic = false);
		GameObject(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, bool a_IsRectangle = true, bool a_IsDynamic = false);
		GameObject(Sprite& a_Sprite, vec2 a_Position);
		~GameObject() = default;
		// member data access
		Sprite& GetSprite() { return m_Sprite; }
		vec2 GetSpriteSize() { return m_SpriteSize; }
		vec2 GetPosition() { return m_Position; }
		vec2 SetPosition(vec2 a_Position) { m_Position = a_Position; }
		vec2 GetCenterPosition() { return m_CenterPosition; }
		vec2 GetVelocity() { return m_Acceleration; }
		vec2 SetVelocity(vec2 a_Velocity) { m_Velocity = a_Velocity; }
		void SetSpeed(float a_Speed) { m_Speed = a_Speed; }
		Collision& GetCollision() { return m_Collision; }
		Shape GetShape() { m_Shape; }
		// special operations
		void Update(Surface* screen);
		void BouncePhysics(Game* game, float& dt);
	protected:
		// attributes
		Sprite& m_Sprite;
		const vec2 m_SpriteSize;
		vec2 m_Position;
		vec2 m_CenterPosition;
		vec2 m_Acceleration;
		vec2 m_Velocity;
		float m_Speed = 1.0f;
		float m_GroundBuffer = 1.0f;
		Collision m_Collision = {false, false};
		bool m_OnScreen = false;
		const Shape m_Shape;
		const ObjectType m_ObjectType;
	};

	class Player : public GameObject {
	public:
		// constructor / destructor
		Player(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_PlayerSpeed);
		// member data access
		// special operations
		void Update(Game* game, Surface* screen, Input* input, float& dt);
		void PlayerCollision(GameObject& obj);
	private:
		// attributes
		float m_Boost = 0;
		float m_BoostPower = 30;
		
	};
}