#include "gameobject.h"

namespace Tmpl8 {

	GameObject::GameObject(Sprite& _sprite, vec2 _position, vec2 _velocity, bool _isRectangle) :
		sprite(_sprite),
		startTransform{_position, _velocity},
		transform{_position, _velocity},
		spriteSize((float) _sprite.GetWidth(), (float) _sprite.GetHeight()) {
	}

	GameObject::GameObject(Sprite& _sprite, vec2 _position, bool _isRectangle) :
		sprite(_sprite),
		startTransform{_position, {0, 0}},
		transform{_position, {0, 0}},
		spriteSize((float) _sprite.GetWidth(), (float) _sprite.GetHeight()) {
	}

	Player::Player(Sprite& _sprite, vec2 _position, vec2 _velocity, float _speed) :
		GameObject(_sprite, _position, _velocity, false),
		speed(_speed) {
	}

	Platform::Platform(Sprite& _sprite, vec2 _position, vec2 _velocity) :
		GameObject(_sprite, _position, _velocity) {
	}

	UI::UI(Sprite& _sprite, vec2 _position, bool _isRectangle) :
		GameObject(_sprite, _position, _isRectangle),
		buttonState(ButtonState::None) {
	}

	// >> GameObject and related special operators << //
	void GameObject::Draw(Surface* screen) {
		// check if the GameObject is on screen, and update the on screen variable
		if (transform.position.x + spriteSize.x > 0 && transform.position.x < ScreenWidth &&
			transform.position.y + spriteSize.y > 0 && transform.position.y < ScreenHeight) {
			onScreen = true;
		} else {
			onScreen = false;
		}

		// only draw the object if it is on screen
		if (onScreen) {
			sprite.Draw(screen, (int) transform.position.x, (int) transform.position.y);
		}
	}

	void GameObject::Reset() {
		// reset the transform
		transform = startTransform;
	}

	void UI::Update(Surface* screen, Input* input) {
		// check if the mouse is above the button, if not set the buttonstate to none
		if (input->GetMousePos().x >= (int)transform.position.x && 
			input->GetMousePos().x <= (int) transform.position.x + sprite.GetWidth() &&
			input->GetMousePos().y >= (int) transform.position.y && 
			input->GetMousePos().y <= (int) transform.position.y + sprite.GetHeight()) {

			// check if a mouse button was pressed, held or released, then set the buttonState accordingly
			if (input->GetMouseButtonDown(1)) {
				buttonState = ButtonState::Pressed;
				sprite.SetFrame(2);
			} else if (input->GetMouseButton(1)) {
				buttonState = ButtonState::Held;
				sprite.SetFrame(2);
			} else if (input->GetMouseButtonUp(1)) {
				buttonState = ButtonState::Released;
				sprite.SetFrame(0);
			} else {
				buttonState = ButtonState::Hover;
				sprite.SetFrame(1);
			}
		} else {
			buttonState = ButtonState::None;
			sprite.SetFrame(0);
		}

		// draw the object
		Draw(screen);
	}

	void Platform::Update(Game* game, Surface* screen, Player& player, float& dt) {
		// move the platform using the velocity
		transform.position += transform.velocity * game->GetPlatformSpeed() * dt;

		// set the platform at a random value on the x axis above the screen
		if (transform.position.y >= ScreenHeight) { 
			float randX = Rand(maxRandX) + 10;

			transform.position = {randX, transform.position.y - 600};
			platformPassed = false;
		}

		// check if you passed the plafrom, if you did add score
		if (!platformPassed && player.transform.position.y <= transform.position.y && game->GetScore() < 99) {
			game->SpeedUp();
			game->ScoreUp();

			platformPassed = true;
		}

		// draw the object
		Draw(screen);
	}

	void Player::Update(Game* game, Surface* screen, Input* input, float& dt)  {
		// check if the player hit the bottem of the screen, if it did gameover
		if (transform.position.y + spriteSize.y >= ScreenHeight - groundBuffer) {
			game->GameOver();
		}

		// make the player always be inside the screen in the x axis
		if (transform.position.x > ScreenWidth - spriteSize.x || transform.position.x < 0) {
			transform.position.x = Max(0.0f, Min((float)ScreenWidth - spriteSize.x, transform.position.x));
			transform.velocity.x = -transform.velocity.x;
		}

		// if you hold space and ground collision is false, add velocity on the Y
		if (input->GetKey(SDL_SCANCODE_SPACE) && platformGroundCollision == nullptr) {
     		transform.velocity.y = boostDropForce;
		} else if (input->GetKey(SDL_SCANCODE_SPACE) && platformGroundCollision != nullptr) {
			transform.velocity.x = 0;
			boostReady = true;
		}

		// if you release space and the ground collision is true, update the velocity and acceleration and reset the boost
		if (input->GetKeyUp(SDL_SCANCODE_SPACE) && boostReady) {
			transform.velocity = (input->GetMousePos() - transform.position).normalized() * boostPower;
			boostReady = false;
		}

		// update the player position
		BouncePhysics(game, dt);
		
		// draw the object
		Draw(screen);
	}

	void Player::BouncePhysics(Game* game, float& dt) {
		// add gravity to the Y axis of the velocity
		if (platformGroundCollision == nullptr) {
			transform.velocity.y += (float)game->GetGravity() * dt;
		}

		// add acceleration and deceleration to the velocity
		transform.velocity *= game->GetDeceleration();

		// update the position using the velocity
		transform.position += transform.velocity * speed * dt;

		if (platformGroundCollision != nullptr && boostReady) {
			transform.position.y = platformGroundCollision->transform.position.y - spriteSize.y;
		}

	}

	bool Player::PlatformCollision(Game* game, Platform& platform) {
		vec2 centerPosition = {transform.position.x + spriteSize.x / 2.0f, transform.position.y + spriteSize.y / 2.0f};

		// calculate the closest X and Y point on the obj to the player
		float closestX = Max(platform.transform.position.x, Min(centerPosition.x, platform.transform.position.x + platform.GetSpriteSize().x));
		float closestY = Max(platform.transform.position.y, Min(centerPosition.y, platform.transform.position.y + platform.GetSpriteSize().y));
		
		// calculate the distance between the closest X and Y and the center of the player
		float distanceX = centerPosition.x - closestX;
		float distanceY = centerPosition.y - closestY;
		float distance = std::sqrt((distanceX * distanceX) + (distanceY * distanceY));

		// check if distance is less then the radius of the player
		if (distance <= spriteSize.x / 2.0f) {
			// determine what axis the player hit
			if (abs(distanceX) > abs(distanceY)){
				// invert the x velocity
				transform.velocity.x = -transform.velocity.x;

				// determine the side of the obj the player hit, then set the position to the border of the platform
				if (distanceX > 0) {
					transform.position.x = platform.transform.position.x + platform.GetSpriteSize().x;
				} else {
					transform.position.x = platform.transform.position.x - spriteSize.x;
				}
			} else {
				// invert the y velocity
				transform.velocity.y = -transform.velocity.y;

				// determine the side of the obj the player hit, then set the position to the border of the platform
				if (distanceY > 0) {
					transform.position.y = platform.transform.position.y + platform.GetSpriteSize().y;
				} else {
					transform.position.y = platform.transform.position.y - spriteSize.y;

					// set the platformGroundCollision to the current platform
					platformGroundCollision = &platform;
				}
			}
			return true;
		} else {
			// empty the platformGroundCollision
			if (platformGroundCollision == &platform) {
				platformGroundCollision = nullptr;
			}

			return false;
		}

	}
}