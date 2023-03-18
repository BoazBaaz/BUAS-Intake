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
		vec2 GetVelocity() { return m_Velocity; }
		vec2 SetVelocity(vec2 a_Velocity) { m_Velocity = a_Velocity; }
		void SetSpeed(float a_Speed) { m_Speed = a_Speed; }
		bool GetGroundCollision() { return m_GroundCollision; }
		Shape GetShape() { m_Shape; }
		// special operations
		virtual void Update(Player* player, Game* game, Surface* screen, float& dt);
	protected:
		// special operations
		void Physics(Game* game, float& dt);
		// attributes
		Sprite& m_Sprite;
		const vec2 m_SpriteSize;
		vec2 m_Position;
		vec2 m_CenterPosition;
		vec2 m_Velocity;
		float m_Speed = 1.0f;
		float m_GroundBuffer = 0.5f;
		bool m_GroundCollision = false;
		bool m_OnScreen = false;
		// custom attributes
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
		void CircleRectCollision(GameObject& obj);
	private:
		// attributes
		const float m_MaxBoost = 1;
		float m_Boost = 0;
		float m_BoostPower = 30;
		float m_BoostBuildup = 1.4;
		float m_BoostDropForce = 200;
		
	};
}