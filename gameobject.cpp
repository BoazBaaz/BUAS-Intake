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
		m_SpriteSize((float)a_Sprite.GetWidth(), (float)a_Sprite.GetHeight()) {
	}

	GameObject::GameObject(Sprite& a_Sprite, vec2 a_Position) :
		m_Sprite(a_Sprite),
		m_Position(a_Position),
		m_Shape(Shape::Rectangle),
		m_ObjectType(ObjectType::Static) {
	}

	Player::Player(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, float a_PlayerSpeed) :
		GameObject(a_Sprite, a_Position, a_Velocity, a_PlayerSpeed, false, true) {
	}

	// >> GameObject and related special operators << //

	void GameObject::Update(Player* player, Game* game, Surface* screen, float& dt) {
		// check if the GameObject is on screen, and update the on screen variable
		if (m_Position.x + m_SpriteSize.x > 0 && m_Position.x < ScreenWidth &&
			m_Position.y + m_SpriteSize.y > 0 && m_Position.y < ScreenHeight) {
			m_OnScreen = true;
		} else {
			m_OnScreen = false;
		}

		// update the position if the objectType is Dynamic
		if (m_ObjectType == ObjectType::Dynamic) {
			Physics(game, dt);
		}

		// TODO: Alway have the player x position be the center of the screen
		// TODO: Calcule in the Update if a object is on screen
		// TODO: Draw object relative to the player
		// only draw the object if it is on screen
		if (m_OnScreen) {
			m_Sprite.Draw(screen, (int)m_Position.x, (int)m_Position.y);
		}
	}

	void Player::Update(Game* game, Surface* screen, Input* input, float& dt)  {
		// if you hold space add velocity on the Y axis, once groudn collision is true build up boost 
		if (input->GetKey(SDL_SCANCODE_SPACE) && !m_GroundCollision) {
			m_Velocity.y = m_BoostDropForce;
		} else if (input->GetKey(SDL_SCANCODE_SPACE) && m_GroundCollision) {
			m_Boost = (m_Boost < m_MaxBoost) ? (m_Boost += (m_BoostBuildup * dt)) : m_MaxBoost;
			m_Velocity = { 0, 0 };
		}

		// if you release space and the ground collision is true, update the velocity and reset the boost
		if (input->GetKeyUp(SDL_SCANCODE_SPACE) && m_GroundCollision) {
			m_Velocity = (input->GetMousePos() - m_Position).normalized() * (m_Boost * m_BoostPower);
			m_Boost = 0;
		}

		GameObject::Update(this, game, screen, dt);
	}

	void GameObject::Physics(Game* game, float& dt) {
		m_GroundCollision = (m_Position.y >= (ScreenHeight - m_SpriteSize.y) - m_GroundBuffer) ? true : false;

		// add gravity to the Y axis of the velocity if you do not have ground collision
		if (!m_GroundCollision) {
			m_Velocity.y += (float)game->GetGravity() * dt;
		}

		// add deceleration to the velocity
		m_Velocity *= game->GetDeceleration();

		// update the position using the velocity, then update the centerPosition using the new position
		m_Position += (m_Velocity * m_Speed) * dt;
		m_CenterPosition = { m_Position.x + m_SpriteSize.x / 2.0f, m_Position.y + m_SpriteSize.y / 2.0f };

		// check if the object hit the bottom of the screen, if it did reverse the velocity on the Y axis
		if (m_GroundCollision) {
			m_Position.y = (float)ScreenHeight - m_SpriteSize.y;
			m_Velocity.y = -m_Velocity.y;
		}
	}


	void Player::CircleRectCollision(GameObject& obj) {
		//obj.GetShape() == Shape::Rectangle
		// TODO: Collision check
		// TODO: Only bounce the player when they did not use the psace key to go down
		// TODO: Make a new GetCenterPos and function and remove the old GetPos from this formula

		// calculate the closest X and Y point on the obj to the player
		float closestX = Clamp(m_CenterPosition.x, obj.GetPosition().x, obj.GetPosition().x + obj.GetSpriteSize().x);
		float closestY = Clamp(m_CenterPosition.y, obj.GetPosition().y, obj.GetPosition().y + obj.GetSpriteSize().y);
		std::cout << "Center: " << m_CenterPosition.x << ", " << m_CenterPosition.y << std::endl;
		
		// calculate the distance between the closest X and Y and the center of the player
		float distanceX = m_CenterPosition.x - closestX;
		float distanceY = m_CenterPosition.y - closestY;
		float distance = std::sqrtf(distanceX * distanceX + distanceY * distanceY);;

		std::cout << "?: " << distance << std::endl;
		// if the distance is less than or equals the radius of the player, invert the velocity of the player 
		if (distance <= m_SpriteSize.x / 2.0f) {
			std::cout << "oldVel: " << m_Velocity.x << ", " << m_Velocity.y << std::endl;
			m_Velocity = -m_Velocity;
			std::cout << "newVel: " << m_Velocity.x << ", " << m_Velocity.y << std::endl;
		}
	}
}