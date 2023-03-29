#pragma once

#include "game.h"

namespace Tmpl8 {
	class Input;
	class Game;

	class GameObject { // base class for all other objects
	protected:
		enum class Shape { Rectangle, Circle };
		struct Transform {
			vec2 position;
			vec2 velocity;
		};
	public:
		// constructor / destructor
		GameObject(Sprite& _sprite, vec2 _position, vec2 _velocity, float _speed, bool _isRectangle = true);	// sprite, pos, vel, speed
		GameObject(Sprite& _sprite, vec2 _position, vec2 _velocity, bool _isRectangle = true);					// sprite, pos, vel
		GameObject(Sprite& _sprite, vec2 _position, bool _isRectangle = true);									// sprite, pos
		~GameObject() = default;
		// member data access
		Sprite& GetSprite() { return sprite; }
		vec2 GetSpriteSize() { return spriteSize; }
		void SetPosition(vec2 _position) { transform.position = _position; }
		vec2 GetPosition() { return transform.position; }
		void SetVelocity(vec2 _velocity) { transform.velocity = _velocity; }
		vec2 GetVelocity() { return transform.velocity; }
		void SetSpeed(float _speed) { speed = _speed; }
		// special operations
		void Draw(Surface* screen);
		void Reset();
	protected:
		// attributes
		Sprite& sprite;
		const vec2 spriteSize;
		Transform transform;
		float speed = 1.0f;
		bool onScreen = false;
		const Shape shape;
		// re-initializer values
		const Transform startTransform;
		const float startSpeed = 1.0f;
	};

	class UI : public GameObject {
		enum class ButtonState { None, Hover, Pressed, Held, Released};
	public:
		// constructor / destructor
		UI(Sprite& _sprite, vec2 _position, bool a_IsRectangle = true);
		~UI() = default;
		// member data access
		bool Pressed() { return buttonState == ButtonState::Pressed; }
		bool Released() { return buttonState == ButtonState::Released; }
		// special operations
		void Update(Surface* screen, Input* input);
	private:
		// attributes
		ButtonState buttonState;
	};

	class Platform : public GameObject {
	public:
		// constructor / destructor
		Platform(Sprite& _sprite, vec2 _position, vec2 _velocity, float _speed);
		~Platform() = default;
		// member data access
		void SetPlatformPassed(bool _platformHit) { platformPassed = _platformHit; }
		bool GetPlatformPassed() { return platformPassed; }
		// special operations
		void Update(Surface* screen, float& dt);
	private:
		// attributes
		float maxRandX = (ScreenWidth - spriteSize.x) - 20;
		bool platformPassed = false;
	};

	class Player : public GameObject {
	public:
		// constructor / destructor
		Player(Sprite& _sprite, vec2 _position, vec2 _velocity, float _speed);
		Player() = default;
		~Player() = default;
		// member data access
		bool GetGroundCollision() { return groundCollision; }
		// special operations
		void Update(Game* game, Surface* screen, Input* input, float& dt);
		void BouncePhysics(Game* game, float& dt);
		void PlatformCollision(Game* game, Platform& a_CollisionObject);
	private:
		// attributes
		bool boostAvailable = false;
		float boostPower = 30;
		float boostDropForce = 30;
		float groundBuffer = 0.2f;
		bool groundCollision = false;
	};
}