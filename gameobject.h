#pragma once

#include "game.h"

namespace Tmpl8 {
	class Game;
	class Input;
	class Player;

	class GameObject {
	protected:
		enum class ObjectType { Static, Dynamic }; 
		enum class Shape { Rectangle, Circle };
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
		Shape GetShape() { m_Shape; }
		// special operations
		void Update(Surface* screen, float& dt);
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
		const ObjectType m_ObjectType;
	};

	class Player : public GameObject {
	public:
		// constructor / destructor
		Player(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_PlayerSpeed);
		// member data access
		bool GetGroundCollision() { return m_GroundCollision; }
		// special operations
		void Update(Game* game, Surface* screen, Input* input, float& dt);
		void BouncePhysics(Game* game, float& dt);
		void PlayerCollision(GameObject& a_CollisionObject);
	private:
		// attributes
		bool m_Boost = false;
		float m_BoostPower = 30;
		float m_BoostDropForce = 30;
		float m_GroundBuffer = 0.2f;
		bool m_GroundCollision = false;
		
	};
}