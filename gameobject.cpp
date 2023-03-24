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

	void GameObject::Update(Surface* screen, float& dt) {
		// check if the GameObject is on screen, and update the on screen variable
		if (m_Position.x + m_SpriteSize.x > 0 && m_Position.x < ScreenWidth &&
			m_Position.y + m_SpriteSize.y > 0 && m_Position.y < ScreenHeight) {
			m_OnScreen = true;
		} else {
			m_OnScreen = false;
		}

		// update the gameObjects position
		if (m_ObjectType == ObjectType::Dynamic) {
			m_Position += m_Velocity * m_Speed * dt;
		}

		//update the center position
		m_CenterPosition = { m_Position.x + m_SpriteSize.x / 2.0f, m_Position.y + m_SpriteSize.y / 2.0f };

		// only draw the object if it is on screen
		if (m_OnScreen) {
			m_Sprite.Draw(screen, (int)m_Position.x, (int)m_Position.y);
		}
	}

	void Player::Update(Game* game, Surface* screen, Input* input, float& dt)  {
		// check if the GameObject is on screen, and update the on screen variable
		if (m_Position.x + m_SpriteSize.x > 0 && m_Position.x < ScreenWidth &&
			m_Position.y + m_SpriteSize.y > 0 && m_Position.y < ScreenHeight) {
			m_OnScreen = true;
		}
		else {
			m_OnScreen = false;
		}

		// check if the player hit the bottem of the screen and set the groundCollision accordingly
		if (m_Position.y + m_SpriteSize.y >= ScreenHeight - m_GroundBuffer) {
			// invert the velocity
			m_Velocity.y = -m_Velocity.y;
			
			//set the position to the "ground"
			m_Position.y = (float)ScreenHeight - m_SpriteSize.y;

			m_GroundCollision = true;
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

		// only draw the object if it is on screen
		if (m_OnScreen) {
			m_Sprite.Draw(screen, (int)m_Position.x, (int)m_Position.y);
		}
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

		//updat the center position
		m_CenterPosition = {m_Position.x + m_SpriteSize.x / 2.0f, m_Position.y + m_SpriteSize.y / 2.0f};

		// reset ground collision
		m_GroundCollision = false;
	}

	void Player::PlayerCollision(GameObject& a_CollisionObject) {
		// calculate the closest X and Y point on the obj to the player
		float closestX = std::max(a_CollisionObject.GetPosition().x, std::min(m_CenterPosition.x, a_CollisionObject.GetPosition().x + a_CollisionObject.GetSpriteSize().x));
		float closestY = std::max(a_CollisionObject.GetPosition().y, std::min(m_CenterPosition.y, a_CollisionObject.GetPosition().y + a_CollisionObject.GetSpriteSize().y));
		
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

				// determain the side of the obj the player hit, then set the position to the bound of the obj
				if (distanceX > 0) {
					m_Position.x = a_CollisionObject.GetPosition().x + a_CollisionObject.GetSpriteSize().x;
				} else {
					m_Position.x = a_CollisionObject.GetPosition().x - m_SpriteSize.x;
				}
			} else {
				// set the ground collision to true
				m_GroundCollision = true;

				// invert the y velocity
				m_Velocity.y = -m_Velocity.y;

				// determain the side of the obj the player hit, then set the position to the bound of the obj
				if (distanceY > 0) {
					m_Position.y = a_CollisionObject.GetPosition().y + a_CollisionObject.GetSpriteSize().y;
				} else {
					m_Position.y = a_CollisionObject.GetPosition().y - m_SpriteSize.y;
				}
			}
		}
	}
}