#include "gameobject.h"

namespace Tmpl8 {

	GameObject::GameObject(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, bool a_IsDynamic) :
		m_Sprite(a_Sprite),
		m_Position(a_Position),
		m_Velocity(a_Velocity),
		m_ObjectType(a_IsDynamic ? ObjectType::Dynamic : ObjectType::Static) {
	}

	GameObject::GameObject(Sprite& a_Sprite, vec2 a_Position) :
		m_Sprite(a_Sprite),
		m_Position(a_Position),
		m_ObjectType(ObjectType::Static) {
	}

	void GameObject::Update(Game* game, Surface* screen, float& dt) {
		// Update the position if this is a dynamic GameObject
		if (m_ObjectType == ObjectType::Dynamic) {
			// Update the velocity
			m_Velocity *= game->GetDeceleration();

			// Update position
			m_Position += (m_Velocity * m_Speed) * dt;
		}

		// TODO: Draw object relative to the player
		// Only draw the object if it is on the screen
		if (m_Position.x < ScreenWidth && m_Position.x + m_Sprite.GetWidth() > 0 &&
			m_Position.y < ScreenHeight && m_Position.y + m_Sprite.GetHeight() > 0) {
			m_Sprite.Draw(screen, (int)m_Position.x, (int)m_Position.y);
		}
	}

	Player::Player(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_PlayerSpeed) :
		GameObject(a_Sprite, a_Position, a_Velocity, true) {
		SetSpeed(a_PlayerSpeed);
	}

	void Player::Update(Game* game, Surface* screen, Input* input, float& dt) {
		// Update velocity down and reset velocity when groundHit
		if (input->GetKey(SDL_SCANCODE_SPACE) && !m_GroundCollision) {
			player.GetVelocity().y = 100;
		}
		else if (input->GetKey(SDL_SCANCODE_SPACE) && m_GroundCollision) {
			boost = (boost < maxBoost) ? (boost += dt) : maxBoost;
			player.GetVelocity() = { 0, 0 };
		}

		// Update velocity and reset boost
		if (input->GetKeyUp(SDL_SCANCODE_SPACE) && m_GroundCollision) {
			player.GetVelocity() = (input->GetMousePos() - player.GetPosition()) / 100;
			player.GetVelocity() *= boost;

			boost = 0;
		}

		// Update the position of the player
		if (m_ObjectType == ObjectType::Dynamic) {
			// Ground check
			m_GroundCollision = (m_Position.y >= ScreenHeight - m_Sprite.GetHeight() - 1) ? true : false;

			// Apply gravity to the velocity 
			if (!m_GroundCollision) {
				m_Velocity.y += game->GetGravity() * dt;
			}

			// Update the velocity
			m_Velocity *= game->GetDeceleration();

			// Update position
			m_Position += (m_Velocity * m_Speed) * dt;
		}

		// TODO: Collision check
		if (m_Position.y + m_Sprite.GetHeight() > ScreenHeight) {
			m_Position.y = (float)ScreenHeight - m_Sprite.GetHeight();
			m_Velocity.y = -m_Velocity.y;
		}

		// TODO: Alway have the player x position be the center of the screen
		// Only draw the object if it is on the screen
		if (m_Position.x < ScreenWidth && m_Position.x + m_Sprite.GetWidth() > 0 &&
			m_Position.y < ScreenHeight && m_Position.y + m_Sprite.GetHeight() > 0) {
			m_Sprite.Draw(screen, (int)m_Position.x, (int)m_Position.y);
		}
	}
}