#include "game.h"

namespace Tmpl8 {
	GameObject::GameObject(Sprite* a_Sprite, vec2 a_Position, vec2 a_Velocity, bool a_IsDynamic) :
		m_Sprite(a_Sprite),
		m_Position(a_Position),
		m_Velocity(a_Velocity),
		m_ObjectType((GameObject::ObjectType)a_IsDynamic) {
	}

	GameObject::GameObject(Sprite* a_Sprite, vec2 a_Position) :
		m_Sprite(a_Sprite),
		m_Position(a_Position), 
		m_ObjectType(ObjectType::Static) {
	}

	GameObject::~GameObject() {
		delete m_Sprite;
	}

	// Variables (Main Scene)
	GameObject startButton(new Sprite(new Surface("assets/start_button.tga"), 2), vec2(300, 412));

	// Variables (Game Scene)
	GameObject player(new Sprite(new Surface("assets/ball.png"), 1), vec2(375, 462), vec2(4, -20), true);
	GameObject platform(new Sprite(new Surface("assets/balk.png"), 1), vec2(500, 200));

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
	void Game::Tick(float dt) {
		screen->Clear(0);

		switch (curScene) {
			case main:
				if (Button(startButton)) {
					ChangeScene(game);
				}
				break;
			case game:

				// Update velocity down and reset velocity when groundHit
				if (input->GetKey(SDL_SCANCODE_SPACE) && !groundHit) {
					player.GetVelocity().y = 100;
				}
				if (input->GetKey(SDL_SCANCODE_SPACE) && groundHit) {
					boost = (boost < maxBoost) ? (boost += dt) : maxBoost;
					player.GetVelocity() = {0, 0};
				}

				// Update velocity and reset boost
				if (input->GetKeyUp(SDL_SCANCODE_SPACE) && groundHit) {
					player.GetVelocity() = (input->GetMousePos() - player.GetPosition()) / 100;
					player.GetVelocity() *= boost;

					boost = 0;
				}

				Physics(player, dt);
				Collision(player);

				player.GetSprite()->Draw(screen, (int) player.GetPosition().x, (int) player.GetPosition().y);
				platform.GetSprite()->Draw(screen, (int)platform.GetPosition().x, (int)platform.GetPosition().y);
				break;
			default:
				break;
		}
	}

	/*bool CheckCollision(const Circle& circle, const Rect& rect) {
		float closestX = std::max(rect.x, std::min(circle.x, rect.x + rect.width));
		float closestY = std::max(rect.y, std::min(circle.y, rect.y + rect.height));
		float distance = sqrt(pow(circle.x - closestX, 2) + pow(circle.y - closestY, 2));
		return distance <= circle.radius;
	}*/

	void Game::Physics(GameObject& p, float dt) {
		// Ground check
	 	groundHit = (p.GetPosition().y >= ScreenHeight - p.GetSprite()->GetHeight() - 1) ? true : false;

		// Apply gravity to the velocity 
		if (!groundHit) {
			p.GetVelocity().y += (float)gravity * dt;
		}

		// Update the velocity
		p.GetVelocity() *= (float)deceleration;

		// Update position
		p.GetPosition() += p.GetVelocity();
	}

	void Game::Collision(GameObject& p) {
		// Screen boundry collision
		if (p.GetPosition().x < 0 || p.GetPosition().x + p.GetSprite()->GetWidth() > ScreenWidth) {
			p.GetPosition().x = std::max((float) 0, std::min(p.GetPosition().x, (float) ScreenWidth - p.GetSprite()->GetWidth()));
			p.GetVelocity().x = -p.GetVelocity().x;
		}
		if (p.GetPosition().y < 0 || p.GetPosition().y + p.GetSprite()->GetHeight() > ScreenHeight) {
			p.GetPosition().y = std::max((float) 0, std::min(p.GetPosition().y, (float) ScreenHeight - p.GetSprite()->GetHeight()));
			p.GetVelocity().y = -p.GetVelocity().y;
		}

		// Object collision
		// TODO: Array of all object on screen
		/*for each (object var in collection_to_loop) {
		}*/
		if (p.GetPosition().x <= platform.GetPosition().x + platform.GetSprite()->GetWidth() &&
			p.GetPosition().x + p.GetSprite()->GetWidth() >= platform.GetPosition().x &&
			p.GetPosition().y <= platform.GetPosition().y + platform.GetSprite()->GetHeight() &&
			p.GetPosition().y + p.GetSprite()->GetHeight() >= platform.GetPosition().y) {
			
			p.GetVelocity().x = -p.GetVelocity().x;
			p.GetVelocity().y = -p.GetVelocity().y;
		}
	}

	bool Game::Button(GameObject sObj) {
		sObj.GetSprite()->SetFrame(0);
		sObj.GetSprite()->Draw(screen, (int)sObj.GetPosition().x, (int) sObj.GetPosition().y);
		if (input->GetMousePos().x >= (int) sObj.GetPosition().x && input->GetMousePos().x <= (int) sObj.GetPosition().x + sObj.GetSprite()->GetWidth()) {
			if (input->GetMousePos().y >= (int) sObj.GetPosition().y && input->GetMousePos().y <= (int) sObj.GetPosition().y + sObj.GetSprite()->GetHeight()) {
				if (input->GetMouseButton(1)) {
					sObj.GetSprite()->SetFrame(1);
					sObj.GetSprite()->Draw(screen, (int) sObj.GetPosition().x, (int) sObj.GetPosition().y);
				}
				if (input->GetMouseButtonUp(1)) {
					return true;
				}
			}
		}
		return false;
	}
};