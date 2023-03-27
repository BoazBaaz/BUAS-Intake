#include "gameobject.h"

namespace Tmpl8 {

	GameObject::GameObject(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_Speed, bool a_IsRectangle) :
		m_Sprite(a_Sprite),
		m_Position(a_Position),
		m_Velocity(a_Velocity),
		m_Speed(a_Speed),
		m_Shape(a_IsRectangle ? Shape::Rectangle : Shape::Circle),
		m_SpriteSize((float)a_Sprite.GetWidth(), (float)a_Sprite.GetHeight()) {
	}

	GameObject::GameObject(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, bool a_IsRectangle) :
		m_Sprite(a_Sprite),
		m_Position(a_Position),
		m_Velocity(a_Velocity),
		m_Shape(a_IsRectangle ? Shape::Rectangle : Shape::Circle),
		m_SpriteSize((float) a_Sprite.GetWidth(), (float) a_Sprite.GetHeight()) {
	}

	GameObject::GameObject(Sprite& a_Sprite, vec2 a_Position, bool a_IsRectangle) :
		m_Sprite(a_Sprite),
		m_Position(a_Position),
		m_Shape(a_IsRectangle ? Shape::Rectangle : Shape::Circle),
		m_SpriteSize((float) a_Sprite.GetWidth(), (float) a_Sprite.GetHeight()) {
	}

	Player::Player(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_Speed) :
		GameObject(a_Sprite, a_Position, a_Velocity, a_Speed, false) {
	}

	Platform::Platform(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_Speed) :
		GameObject(a_Sprite, a_Position, a_Velocity, a_Speed) {
	}

	UI::UI(Sprite& a_Sprite, vec2 a_Position, bool a_IsRectangle) :
		GameObject(a_Sprite, a_Position, a_IsRectangle),
		m_ButtonState(ButtonState::None) {
	}

	// >> GameObject and related special operators << //
	void GameObject::Draw(Surface* screen) {
		// check if the GameObject is on screen, and update the on screen variable
		if (m_Position.x + m_SpriteSize.x > 0 && m_Position.x < ScreenWidth &&
			m_Position.y + m_SpriteSize.y > 0 && m_Position.y < ScreenHeight) {
			m_OnScreen = true;
		}
		else {
			m_OnScreen = false;
		}

		// only draw the object if it is on screen
		if (m_OnScreen) {
			m_Sprite.Draw(screen, (int) m_Position.x, (int) m_Position.y);
		}
	}

	void UI::Update(Surface* screen, Input* input) {
		// check if the mouse is above the button, if not set the buttonstate to none
		if (input->GetMousePos().x >= (int)m_Position.x && 
			input->GetMousePos().x <= (int) m_Position.x + m_Sprite.GetWidth() &&
			input->GetMousePos().y >= (int) m_Position.y && 
			input->GetMousePos().y <= (int) m_Position.y + m_Sprite.GetHeight()) {

			// check if a mouse button was pressed, held or released, then set the buttonState accordingly
			if (input->GetMouseButtonDown(1)) {
				m_ButtonState = ButtonState::Pressed;
				m_Sprite.SetFrame(1);
			} else if (input->GetMouseButton(1)) {
				m_ButtonState = ButtonState::Held;
				m_Sprite.SetFrame(1);
			} else if (input->GetMouseButtonUp(1)) {
				m_ButtonState = ButtonState::Released;
				m_Sprite.SetFrame(0);
			} else {
				m_ButtonState = ButtonState::Hover;
				m_Sprite.SetFrame(0); // possibility to add a frame for when you hover over the button
			}
		} else {
			m_ButtonState = ButtonState::None;
			m_Sprite.SetFrame(0);
		}

		// draw the object
		Draw(screen);
	}

	void Platform::Update(Surface* screen, float& dt) {
		// move the platform using the velocity
		m_Position += m_Velocity * m_Speed * dt;

		//update the center position
		m_CenterPosition = {m_Position.x + m_SpriteSize.x / 2.0f, m_Position.y + m_SpriteSize.y / 2.0f};

		// seth the platform at a random value on the x axis above the screen
		if (m_Position.y >= ScreenHeight) { 
			float randX = Rand(m_MaxRand) + 10;

			m_Position = {randX, m_Position.y - 600};
			m_PlatformHit = false;
		}

		// draw the object
		Draw(screen);
	}

	void Player::Update(Game* game, Surface* screen, Input* input, float& dt)  {
		// check if the player hit the bottem of the screen, if it did gameover
		if (m_Position.y + m_SpriteSize.y >= ScreenHeight - m_GroundBuffer) {
			game->Gameover();
		}

		// make the player always be inside the screen in the x axis
		if (m_Position.x > ScreenWidth) {
			m_Position.x = 0 - m_SpriteSize.x;
		} else if (m_Position.x + m_SpriteSize.x < 0) {
			m_Position.x = ScreenWidth;
		}

		// if you hold space add velocity on the Y axis, once groudn collision is true build up boost 
		if (input->GetKey(SDL_SCANCODE_SPACE)) {
			if (!m_GroundCollision) {
     			m_Velocity.y = m_BoostDropForce;
			} else {
				m_Boost = true;
				m_Velocity = { 0, 0 };
			}
		}

		// if you release space and the ground collision is true, update the velocity and acceleration and reset the boost
		if (input->GetKeyUp(SDL_SCANCODE_SPACE) && m_GroundCollision) {
			m_Velocity = (input->GetMousePos() - m_Position).normalized() * (m_Boost * m_BoostPower);
			m_Boost = false;
		}

		// update the player position
		BouncePhysics(game, dt);
		
		// draw the object
		Draw(screen);
	}

	void Player::BouncePhysics(Game* game, float& dt) {
		// add gravity to the Y axis of the velocity
		if (!m_GroundCollision) {
			m_Velocity.y += (float)game->GetGravity() * dt;
		}

		// add acceleration and deceleration to the velocity
		m_Velocity *= game->GetDeceleration();

		// update the position using the velocity, then update the centerPosition using the new position
		m_Position += m_Velocity * m_Speed * dt;

		// update the center position
		m_CenterPosition = {m_Position.x + m_SpriteSize.x / 2.0f, m_Position.y + m_SpriteSize.y / 2.0f};

		// reset ground collision
		m_GroundCollision = false;
	}

	void Player::PlatformCollision(Game* game, Platform& a_Platform) {
		// calculate the closest X and Y point on the obj to the player
		float closestX = std::max(a_Platform.GetPosition().x, std::min(m_CenterPosition.x, a_Platform.GetPosition().x + a_Platform.GetSpriteSize().x));
		float closestY = std::max(a_Platform.GetPosition().y, std::min(m_CenterPosition.y, a_Platform.GetPosition().y + a_Platform.GetSpriteSize().y));
		
		// calculate the distance between the closest X and Y and the center of the player
		float distanceX = m_CenterPosition.x - closestX;
		float distanceY = m_CenterPosition.y - closestY;
		float distance = std::sqrt((distanceX * distanceX) + (distanceY * distanceY));

		// check if distance is less then the radius of the player
		if (distance <= m_SpriteSize.x / 2.0f) {
			// determine what axis the player hit
			if (std::abs(distanceX) > std::abs(distanceY)){
				// invert the x velocity
				m_Velocity.x = -m_Velocity.x;

				// determine the side of the obj the player hit, then set the position to the border of the platform
				if (distanceX > 0) {
					m_Position.x = a_Platform.GetPosition().x + a_Platform.GetSpriteSize().x;
				} else {
					m_Position.x = a_Platform.GetPosition().x - m_SpriteSize.x;
				}
			} else {
				// invert the y velocity
				m_Velocity.y = -m_Velocity.y;

				// determine the side of the obj the player hit, then set the position to the border of the platform
				if (distanceY > 0) {
					m_Position.y = a_Platform.GetPosition().y + a_Platform.GetSpriteSize().y;
				} else {
					// set the ground collision to true
					m_GroundCollision = true;

					// check if you have hit this platform, if not add score and set platformhit to true
					if (!a_Platform.GetPlatformHit()) {
						game->ScoreUp();
						a_Platform.SetPlatformHit(true);
					}

					m_Position.y = a_Platform.GetPosition().y - m_SpriteSize.y;
				}
			}
		}
	}
}