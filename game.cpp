#include "game.h"

namespace Tmpl8 {
	////////////////////////
	//		GAMEOBJECT	  //
	////////////////////////

	GameObject::GameObject(Sprite& a_Sprite, vec2 a_Position, vec2 a_Velocity, bool a_IsDynamic) :
		m_Sprite(a_Sprite),
		m_Position(a_Position),
		m_Velocity(a_Velocity),
		m_ObjectType((GameObject::ObjectType)a_IsDynamic) {
	}

	GameObject::GameObject(Sprite& a_Sprite, vec2 a_Position) :
		m_Sprite(a_Sprite),
		m_Position(a_Position), 
		m_ObjectType(ObjectType::Static) {
	}

	//TODO: Remove the groundHit
	//TODO: Add speed variable
	void GameObject::Update(Surface* screen, float& dt, const float& gravity, const float& deceleration) {
		// Update the position if this is a dynamic GameObject
		if (m_ObjectType == ObjectType::Dynamic) {
			// Ground check
			m_GroundHit = (m_Position.y >= ScreenHeight - m_Sprite.GetHeight() - 1) ? true : false;

			// Apply gravity to the velocity 
			if (!m_GroundHit) {
				m_Velocity.y += gravity * dt;
			}

			// Update the velocity
			m_Velocity *= deceleration;

			// Update position
			m_Position += (m_Velocity * 10) * dt;
		}

		// TODO: Collision check
		if (m_Position.y + m_Sprite.GetHeight() > ScreenHeight) {
			m_Position.y = ScreenHeight - m_Sprite.GetHeight();
			m_Velocity.y = -m_Velocity.y;
		}

		// Only draw the object if it is on the screen
		if (m_Position.x < ScreenWidth && m_Position.x + m_Sprite.GetWidth() > 0 &&
			m_Position.y < ScreenHeight && m_Position.y + m_Sprite.GetHeight() > 0) {
			m_Sprite.Draw(screen, (int)m_Position.x, (int)m_Position.y);
		}
	}

	////////////////////
	//		GAME	  //
	////////////////////

	// Variables (Main Scene)
	Sprite s_StartButton(new Surface("assets/start_button.tga"), 2);
	GameObject startButton(s_StartButton, vec2(300, 412));

	// Variables (Game Scene)
	Sprite s_Player(new Surface("assets/ball.png"), 1);
	GameObject player(s_Player, vec2(375, 462), vec2(4, -20), true);
	Sprite s_Platform(new Surface("assets/balk.png"), 1);
	GameObject platform(s_Platform, vec2(500, 200));

	GameObject gameObjects[2] = {player, platform};

	constexpr float maxBoost = 2;
	float boost = 0;
	bool groundHit = false;

	// On start
	void Game::Init() {
	}

	// On close
	void Game::Shutdown() {
	}

	// Every frame
	void Game::Tick(float& dt) {
		screen->Clear(0);

		switch (m_Scene) {
			case Scene::Main:
				if (Button(startButton)) {
					ChangeScene(Scene::Game);
				}
				break;
			case Scene::Game:

				//TODO: make the groundHit only check the player GameObject
				// Update velocity down and reset velocity when groundHit
				if (input->GetKey(SDL_SCANCODE_SPACE) && !groundHit) {
					player.GetVelocity().y = 100;
				} else if (input->GetKey(SDL_SCANCODE_SPACE) && groundHit) {
					boost = (boost < maxBoost) ? (boost += dt) : maxBoost;
					player.GetVelocity() = {0, 0};
				}

				// Update velocity and reset boost
				if (input->GetKeyUp(SDL_SCANCODE_SPACE) && groundHit) {
					player.GetVelocity() = (input->GetMousePos() - player.GetPosition()) / 100;
					player.GetVelocity() *= boost;

					boost = 0;
				}

				for (GameObject& gameObject : gameObjects) {
					gameObject.Update(screen, dt, m_Gravity, m_Deceleration);
				}
				break;
			default:
				break;
		}
	}

	//void Game::Collision(GameObject& p) {
	//	// Screen boundry collision
	//	if (p.GetPosition().x < 0 || p.GetPosition().x + p.GetSprite().GetWidth() > ScreenWidth) {
	//		p.GetPosition().x = std::max((float) 0, std::min(p.GetPosition().x, (float) ScreenWidth - p.GetSprite().GetWidth()));
	//		p.GetVelocity().x = -p.GetVelocity().x;
	//	}
	//	if (p.GetPosition().y < 0 || p.GetPosition().y + p.GetSprite().GetHeight() > ScreenHeight) {
	//		p.GetPosition().y = std::max((float) 0, std::min(p.GetPosition().y, (float) ScreenHeight - p.GetSprite().GetHeight()));
	//		p.GetVelocity().y = -p.GetVelocity().y;
	//	}

	//	// Object collision
	//	// TODO: Array of all object on screen
	//	/*for each (object var in collection_to_loop) {
	//	}*/
	//	if (p.GetPosition().x <= platform.GetPosition().x + platform.GetSprite().GetWidth() &&
	//		p.GetPosition().x + p.GetSprite().GetWidth() >= platform.GetPosition().x &&
	//		p.GetPosition().y <= platform.GetPosition().y + platform.GetSprite().GetHeight() &&
	//		p.GetPosition().y + p.GetSprite().GetHeight() >= platform.GetPosition().y) {
	//		
	//		p.GetVelocity().x = -p.GetVelocity().x;
	//		p.GetVelocity().y = -p.GetVelocity().y;
	//	}
	//}

	bool Game::Button(GameObject sObj) {
		sObj.GetSprite().SetFrame(0);
		sObj.GetSprite().Draw(screen, (int)sObj.GetPosition().x, (int) sObj.GetPosition().y);
		if (input->GetMousePos().x >= (int) sObj.GetPosition().x && input->GetMousePos().x <= (int) sObj.GetPosition().x + sObj.GetSprite().GetWidth()) {
			if (input->GetMousePos().y >= (int) sObj.GetPosition().y && input->GetMousePos().y <= (int) sObj.GetPosition().y + sObj.GetSprite().GetHeight()) {
				if (input->GetMouseButton(1)) {
					sObj.GetSprite().SetFrame(1);
					sObj.GetSprite().Draw(screen, (int) sObj.GetPosition().x, (int) sObj.GetPosition().y);
				}
				if (input->GetMouseButtonUp(1)) {
					return true;
				}
			}
		}
		return false;
	}
};