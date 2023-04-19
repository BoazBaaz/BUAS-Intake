#include "gameobject.h"

namespace Tmpl8 {

	GameObject::GameObject(Sprite& _sprite, vec2 _position, vec2 _velocity, float _speed, bool _isRectangle) :
		sprite(_sprite),
		startTransform{_position, _velocity},
		transform{_position, _velocity},
		startSpeed(_speed),
		speed(_speed),
		shape(_isRectangle ? Shape::Rectangle : Shape::Circle),
		spriteSize((float)_sprite.GetWidth(), (float)_sprite.GetHeight()) {
	}

	GameObject::GameObject(Sprite& _sprite, vec2 _position, vec2 _velocity, bool _isRectangle) :
		sprite(_sprite),
		startTransform{_position, _velocity},
		transform{_position, _velocity},
		shape(_isRectangle ? Shape::Rectangle : Shape::Circle),
		spriteSize((float) _sprite.GetWidth(), (float) _sprite.GetHeight()) {
	}

	GameObject::GameObject(Sprite& _sprite, vec2 _position, bool _isRectangle) :
		sprite(_sprite),
		startTransform{_position, {0, 0}},
		transform{_position, {0, 0}},
		shape(_isRectangle ? Shape::Rectangle : Shape::Circle),
		spriteSize((float) _sprite.GetWidth(), (float) _sprite.GetHeight()) {
	}

	Player::Player(Sprite& _sprite, vec2 _position, vec2 _velocity, float _speed) :
		GameObject(_sprite, _position, _velocity, _speed, false) {
	}

	Platform::Platform(Sprite& _sprite, vec2 _position, vec2 _velocity, float _speed) :
		GameObject(_sprite, _position, _velocity, _speed) {
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
		// reset the transform and speed
		transform = startTransform;
		speed = startSpeed;
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
		transform.position += transform.velocity * speed * dt;

		// seth the platform at a random value on the x axis above the screen
		if (transform.position.y >= ScreenHeight) { 
			float randX = Rand(maxRandX) + 10;

			transform.position = {randX, transform.position.y - 600};
			platformPassed = false;
		}

		// check if you passed the plafrom, if you did add score
		if (!platformPassed && player.GetTransform().position.y <= transform.position.y) {
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
		if (transform.position.x > ScreenWidth) {
			transform.position.x = 0 - spriteSize.x;
		} else if (transform.position.x + spriteSize.x < 0) {
			transform.position.x = ScreenWidth;
		}

		// if you hold space add velocity on the Y axis, once groudn collision is true build up boost 
		if (input->GetKey(SDL_SCANCODE_SPACE)) {
			if (!groundCollision) {
     			transform.velocity.y = boostDropForce;
			} else {
				boostAvailable = true;
				transform.velocity = { 0, 0 };
			}
		}

		// if you release space and the ground collision is true, update the velocity and acceleration and reset the boost
		if (input->GetKeyUp(SDL_SCANCODE_SPACE) && groundCollision) {
			transform.velocity = (input->GetMousePos() - transform.position).normalized() * (boostAvailable * boostPower);
			boostAvailable = false;
		}

		// update the player position
		BouncePhysics(game, dt);
		
		// draw the object
		Draw(screen);
	}

	void Player::BouncePhysics(Game* game, float& dt) {
		// add gravity to the Y axis of the velocity
		if (!groundCollision) {
			transform.velocity.y += (float)game->GetGravity() * dt;
		}

		// add acceleration and deceleration to the velocity
		transform.velocity *= game->GetDeceleration();

		// update the position using the velocity
		transform.position += transform.velocity * speed * dt;

		// reset ground collision
		groundCollision = false;
	}

	void Player::PlatformCollision(Game* game, Platform& platform) {
		vec2 centerPosition = {transform.position.x + spriteSize.x / 2.0f, transform.position.y + spriteSize.y / 2.0f};

		// calculate the closest X and Y point on the obj to the player
		float closestX = std::max(platform.GetTransform().position.x, std::min(centerPosition.x, platform.GetTransform().position.x + platform.GetSpriteSize().x));
		float closestY = std::max(platform.GetTransform().position.y, std::min(centerPosition.y, platform.GetTransform().position.y + platform.GetSpriteSize().y));
		
		// calculate the distance between the closest X and Y and the center of the player
		float distanceX = centerPosition.x - closestX;
		float distanceY = centerPosition.y - closestY;
		float distance = std::sqrt((distanceX * distanceX) + (distanceY * distanceY));

		// check if distance is less then the radius of the player
		if (distance <= spriteSize.x / 2.0f) {
			// determine what axis the player hit
			if (std::abs(distanceX) > std::abs(distanceY)){
				// invert the x velocity
				transform.velocity.x = -transform.velocity.x;

				// determine the side of the obj the player hit, then set the position to the border of the platform
				if (distanceX > 0) {
					transform.position.x = platform.GetTransform().position.x + platform.GetSpriteSize().x;
				} else {
					transform.position.x = platform.GetTransform().position.x - spriteSize.x;
				}
			} else {
				// invert the y velocity
				transform.velocity.y = -transform.velocity.y;

				// determine the side of the obj the player hit, then set the position to the border of the platform
				if (distanceY > 0) {
					transform.position.y = platform.GetTransform().position.y + platform.GetSpriteSize().y;
				} else {
					// set the ground collision to true
					groundCollision = true;

					transform.position.y = platform.GetTransform().position.y - spriteSize.y;
				}
			}
		}
	}
}