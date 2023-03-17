#include "gameobject.h"

namespace Tmpl8 {

	GameObject::GameObject(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, bool a_IsRectangle, bool a_IsDynamic) :
		m_Sprite(a_Sprite),
		m_Position(a_Position),
		m_Velocity(a_Velocity),
		m_Shape(a_IsRectangle ? Shape::Rectangle : Shape::Circle),
		m_ObjectType(a_IsDynamic ? ObjectType::Dynamic : ObjectType::Static) {
	}

	GameObject::GameObject(Sprite& a_Sprite, vec2 a_Position) :
		m_Sprite(a_Sprite),
		m_Position(a_Position),
		m_Shape(Shape::Rectangle),
		m_ObjectType(ObjectType::Static) {
	}

	void GameObject::Update(Game* game, Surface* screen, float& dt) {
		// update the position if this is a dynamic GameObject
		if (m_ObjectType == ObjectType::Dynamic) {
			Physics(game, dt);
		}

		// TODO: Draw object relative to the player
		// only draw the object if it is on the screen
		if (m_Position.x < ScreenWidth && m_Position.x + m_Sprite.GetWidth() > 0 &&
			m_Position.y < ScreenHeight && m_Position.y + m_Sprite.GetHeight() > 0) {
			m_Sprite.Draw(screen, (int)m_Position.x, (int)m_Position.y);
		}
	}

	void GameObject::Physics(Game* game, float& dt) {
		// update the velocity
		m_Velocity *= game->GetDeceleration();

		// update position
		m_Position += (m_Velocity * m_Speed) * dt;

		// update center position
		m_CenterPosition = { m_Position.x + m_Sprite.GetWidth() / 2.0f, m_Position.y + m_Sprite.GetHeight() / 2.0f };

		// check if the player hit the bottom of the screen, the reverse the velocity
		if (m_Position.y + m_Sprite.GetHeight() > ScreenHeight) {
			m_Position.y = (float)ScreenHeight - m_Sprite.GetHeight();
			m_Velocity.y = -m_Velocity.y;
		}
	}

	Player::Player(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_PlayerSpeed) :
		GameObject(a_Sprite, a_Position, a_Velocity, false, true) {
		SetSpeed(a_PlayerSpeed);
	}

	void Player::Update(Game* game, Surface* screen, Input* input, float& dt) {
		// update velocity down and reset velocity when groundHit
		if (input->GetKey(SDL_SCANCODE_SPACE) && !m_GroundCollision) {
			m_Velocity.y = m_BoostDropForce;
		} else if (input->GetKey(SDL_SCANCODE_SPACE) && m_GroundCollision) {
			m_Boost = (m_Boost < m_MaxBoost) ? (m_Boost += (m_BoostBuildup * dt)) : m_MaxBoost;
			m_Velocity = { 0, 0 };
		}

		// update velocity and reset boost
		if (input->GetKeyUp(SDL_SCANCODE_SPACE) && m_GroundCollision) {
			m_Velocity = (input->GetMousePos() - m_Position).normalized() * (m_Boost * m_BoostPower);
			m_Boost = 0;
		}

		// update the position of the player
		if (m_ObjectType == ObjectType::Dynamic) {
			// ground check
			m_GroundCollision = (m_Position.y + m_Sprite.GetHeight() >= ScreenHeight) ? true : false;

			Physics(game, dt);
		}

		// TODO: Alway have the player x position be the center of the screen
		// Only draw the object if it is on the screen
		if (m_Position.x < ScreenWidth && m_Position.x + m_Sprite.GetWidth() > 0 &&
			m_Position.y < ScreenHeight && m_Position.y + m_Sprite.GetHeight() > 0) {
			m_Sprite.Draw(screen, (int)m_Position.x, (int)m_Position.y);
		}
	}

	void Player::CollisionCheck(GameObject& obj) {
		//obj.GetShape() == Shape::Rectangle
		// TODO: Collision check
		// TODO: Only bounce the player when they did not use the psace key to go down
		// TODO: Make a new GetCenterPos and function and remove the old GetPos from this formula
		float closestX = std::max(obj.GetPosition().x - obj.GetSprite().GetWidth() / 2.f, std::min(m_Position.x, obj.GetPosition().x + obj.GetSprite().GetWidth() / 2.f));
		float closestY = std::max(obj.GetPosition().y - obj.GetSprite().GetHeight() / 2.f, std::min(m_Position.x, obj.GetPosition().y + obj.GetSprite().GetHeight() / 2.f));
		
		// calculate the distance between the closest point and the center of the circle
		float distanceX = m_CenterPosition.x - closestX;
		float distanceY = m_CenterPosition.y - closestY;
		float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

		// if the distance is less than or equal to the radius of the circle, then they are colliding
		if (distance <= circle.radius) {
			return true;
		}
		return false;
	}
}