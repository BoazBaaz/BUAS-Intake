#pragma once

#include "game.h"

namespace Tmpl8 {
	class Game;
	class Input;

	class GameObject {
	protected:
		enum class ObjectType { Static, Dynamic, UI }; 
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
		virtual void Update(Game* game, Surface* screen, float& dt);
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
		bool GetGroundCollision() { return m_GroundCollision; }
		// special operations
		void Update(Game* game, Surface* screen, Input* input, float& dt);
	private:
		// attributes
		const float m_MaxBoost = 1;
		float m_Boost = 0;
		float m_BoostPower = 30;
		float m_BoostBuildup = 1.4;
		float m_BoostDropForce = 200;
		bool m_GroundCollision = false;
	};
}