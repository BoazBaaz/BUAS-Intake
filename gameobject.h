#pragma once

#include "game.h"

namespace Tmpl8 {
	class Input;
	class Game;
	class Player;

	class GameObject { // base class for all other objects
	public:
		struct Transform { vec2 position, velocity; };
		// constructor / destructor
		GameObject(Sprite& _sprite, vec2 _position, vec2 _velocity, float _speed, bool _isRectangle = true);	// sprite, pos, vel, speed
		GameObject(Sprite& _sprite, vec2 _position, vec2 _velocity, bool _isRectangle = true);					// sprite, pos, vel
		GameObject(Sprite& _sprite, vec2 _position, bool _isRectangle = true);									// sprite, pos
		~GameObject() = default;
		// member data access
		Sprite& GetSprite() { return sprite; }
		vec2 GetSpriteSize() { return spriteSize; }
		void SetSpeed(float _speed) { speed = _speed; }
		// special operations
		void Draw(Surface* screen);
		void Reset();
		// attributes
		Transform transform;
	protected:
		// attributes
		Sprite& sprite;
		bool onScreen = false;
		const vec2 spriteSize;
		float speed = 1.0f;
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
		void Update(Game* game, Surface* screen, Player& player, float& dt);
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
		// special operations
		void Update(Game* game, Surface* screen, Input* input, float& dt);
		void BouncePhysics(Game* game, float& dt);
		bool PlatformCollision(Game* game, Platform& a_CollisionObject);
	private:
		// attributes
		bool boostReady = false;
		float boostPower = 30;
		float boostDropForce = 30;
		float groundBuffer = 0.2f;
		Platform* platformGroundCollision = nullptr;
	};
}