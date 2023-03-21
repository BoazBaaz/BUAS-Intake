#include "gameobject.h"

namespace Tmpl8 {

	GameObject::GameObject(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_Speed, bool a_IsRectangle, bool a_IsDynamic) :
		m_Sprite(a_Sprite),
		m_Position(a_Position),
		m_Velocity(a_Velocity),
		m_Speed(a_Speed),
		m_Shape(a_IsRectangle ? Shape::Rectangle : Shape::Circle),
		m_ObjectType(a_IsDynamic ? ObjectType::Dynamic : ObjectType::Static),
		m_SpriteSize((float)a_Sprite.GetWidth(), (float)a_Sprite.GetHeight()) {
	}

	GameObject::GameObject(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, bool a_IsRectangle, bool a_IsDynamic) :
		m_Sprite(a_Sprite),
		m_Position(a_Position),
		m_Velocity(a_Velocity),
		m_Shape(a_IsRectangle ? Shape::Rectangle : Shape::Circle),
		m_ObjectType(a_IsDynamic ? ObjectType::Dynamic : ObjectType::Static),
		m_SpriteSize((float) a_Sprite.GetWidth(), (float) a_Sprite.GetHeight()) {
	}

	GameObject::GameObject(Sprite& a_Sprite, vec2 a_Position) :
		m_Sprite(a_Sprite),
		m_Position(a_Position),
		m_Shape(Shape::Rectangle),
		m_ObjectType(ObjectType::Static),
		m_SpriteSize((float)a_Sprite.GetWidth(), (float)a_Sprite.GetHeight()) {
	}

	Player::Player(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_PlayerSpeed) :
		GameObject(a_Sprite, a_Position, a_Velocity, a_PlayerSpeed, false, true) {
	}

	// >> GameObject and related special operators << //

	void GameObject::Update(Surface* screen) {
		// check if the GameObject is on screen, and update the on screen variable
		if (m_Position.x + m_SpriteSize.x > 0 && m_Position.x < ScreenWidth &&
			m_Position.y + m_SpriteSize.y > 0 && m_Position.y < ScreenHeight) {
			m_OnScreen = true;
		} else {
			m_OnScreen = false;
		}

		// check if you hit the ground
		m_Collision.y = (m_Position.y + m_SpriteSize.y >= ScreenHeight - m_GroundBuffer) ? true : false;

		// only draw the object if it is on screen
		if (m_OnScreen) {
			m_Sprite.Draw(screen, (int)m_Position.x, (int)m_Position.y);
		}
	}

	void Player::Update(Game* game, Surface* screen, Input* input, float& dt)  {
		// update the GameObject member variables
		GameObject::Update(screen);

		// if you hold space add velocity on the Y axis, once groudn collision is true build up boost 
		if (input->GetKey(SDL_SCANCODE_SPACE)) {
			m_Boost = true;
			//m_Collision = { true, true };
			//m_Velocity = { 0, 0 };
		}

		// if you release space and the ground collision is true, update the velocity and acceleration and reset the boost
		if (input->GetKeyUp(SDL_SCANCODE_SPACE)) {
			m_Velocity = (input->GetMousePos() - m_Position).normalized() * m_BoostPower;
			m_Boost = false;
		}

		// update the player position
		BouncePhysics(game, dt);

	}

	void GameObject::BouncePhysics(Game* game, float& dt) {
		// check if you hit something on the X axis, if true inverse X velocity
		if (m_Collision.x) {
			m_Velocity.x = -m_Velocity.x;
		}

		// check if you hit something on the Y axis, if true inverse Y velocity
		// if you are not collisong with something on the Y apply gravity to the velocity.y
		if (m_Collision.y) {
			m_Velocity.y = -m_Velocity.y;
		} else if (!m_Collision.y) {
			m_Velocity.y += (float)game->GetGravity() * dt;
		}

		// add acceleration and deceleration to the velocity
		m_Velocity *= game->GetDeceleration();

		// update the position using the velocity, then update the centerPosition using the new position
		if (!               m_Collision.x || !m_Collision.y) {
			m_Position += (m_Velocity * m_Speed) * dt;
			m_CenterPosition = { m_Position.x + m_SpriteSize.x / 2.0f, m_Position.y + m_SpriteSize.y / 2.0f };
		}
	}

	void Player::PlayerCollision(GameObject& obj) {
		// calculate the closest X and Y point on the obj to the player
		float closestX = std::max(obj.GetPosition().x, std::min(m_CenterPosition.x, obj.GetPosition().x + obj.GetSpriteSize().x));
		float closestY = std::max(obj.GetPosition().y, std::min(m_CenterPosition.y, obj.GetPosition().y + obj.GetSpriteSize().y));
		
		// calculate the distance between the closest X and Y and the center of the player
		float distanceX = m_CenterPosition.x - closestX;
		float distanceY = m_CenterPosition.y - closestY;
		float distance = std::sqrt((distanceX * distanceX) + (distanceY * distanceY));

		// if the distance is less than or equals the radius of the player, invert the velocity of the player 
		if (distance <= m_SpriteSize.x / 2.0f) {
			// determine what side the player hit
			if (std::abs(distanceX) > std::abs(distanceY)) {
				m_Collision.x = true;
			} else {
				m_Collision.y = true;
			}
		} else {
			m_Collision = {false, false};
		};

		

		std::cout << "Pos: " << m_Position.x << ", " << m_Position.y << std::endl;
		std::cout << "Vel: " << m_Velocity.x << ", " << m_Velocity.y << std::endl;
		std::cout << "Col: " << m_Collision.x << ", " << m_Collision.y << std::endl;
		std::cout << "-----------" << std::endl;
	}
}