#include "game.h"

namespace Tmpl8 {

	// Variables (Main Scene)
	Sprite sbSprite(new Surface("assets/start_button.tga"), 2);
	Game::StaticObject startButton(sbSprite, vec2(300, 412));

	// Variables (Game Scene)
	Sprite ballSprite(new Surface("assets/ball.png"), 1);
	Game::DynamicObject player(ballSprite, vec2(375, 462), vec2(4, -20), vec2(0, 0));

	Sprite objSprite(new Surface("assets/balk.png"), 1);
	Game::StaticObject obj(objSprite, vec2(500, 200));

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
					player.acc.y = 100;
					player.vel = {0, 0};
				}
				if (input->GetKey(SDL_SCANCODE_SPACE) && groundHit) {
					boost = (boost < 1) ? (boost + dt * 1) : 1;
					player.vel = {0, 0};
				}

				// Update velocity and reset boost
				if (input->GetKeyUp(SDL_SCANCODE_SPACE)) {
					player.acc = (input->GetMousePos() - player.pos);
					player.acc *= boost;

					boost = 0;
				}

				Physics(player, dt);
				Collision(player);

				player.sprite.Draw(screen, (int)player.pos.x, (int)player.pos.y);
				obj.sprite.Draw(screen, (int)obj.position.x, (int)obj.position.y);
				break;
			default:
				break;
		}
	}

	void Game::Physics(DynamicObject& p, float dt) {
		// Ground check
	 	groundHit = (p.pos.y >= ScreenHeight - p.sprite.GetHeight() - 1) ? true : false;

		std::cout << "Ground: " << groundHit << std::endl;
		std::cout << "Pos: " << p.pos.x << ", " << p.pos.y << std::endl;
		std::cout << "Vel: " << p.vel.x << ", " << p.vel.y << std::endl;

		// Apply gravity to the velocity 
		if (!groundHit) {
			p.vel.y += (float)gravity * dt;
		}

		// Update the velocity
		p.vel += p.acc * dt;
		p.vel *= (float)deceleration;

		// Update position
		p.pos += p.vel;

		// Reset the acceleration for the next caculation
		p.acc = {0, 0};
	}

	void Game::Collision(DynamicObject& p) {
		// Screen boundry collision
		if (p.pos.x < 0 || p.pos.x + p.sprite.GetWidth() > ScreenWidth) {
			p.pos.x = std::max((float) 0, std::min(p.pos.x, (float) ScreenWidth - p.sprite.GetWidth()));
			p.vel.x = -p.vel.x;
		}
		if (p.pos.y < 0 || p.pos.y + p.sprite.GetHeight() > ScreenHeight) {
			p.pos.y = std::max((float) 0, std::min(p.pos.y, (float) ScreenHeight - p.sprite.GetHeight()));
			p.vel.y = -p.vel.y;
		}

		// Object collision
		// TODO: Array of all object on screen
		/*for each (object var in collection_to_loop) {
		}*/
		if (p.pos.x < obj.position.x + obj.sprite.GetWidth() &&
			p.pos.x + p.sprite.GetWidth() > obj.position.x &&
			p.pos.y < obj.position.y + obj.sprite.GetHeight() &&
			p.pos.y + p.sprite.GetHeight() > obj.position.y) {

			p.vel.x = -p.vel.x;
			p.vel.y = -p.vel.y;
		}
	}

	bool Game::Button(StaticObject sObj) {
		sObj.sprite.SetFrame(0);
		sObj.sprite.Draw(screen, (int)sObj.position.x, (int) sObj.position.y);
		if (input->GetMousePos().x >= (int) sObj.position.x && input->GetMousePos().x <= (int) sObj.position.x + sObj.sprite.GetWidth()) {
			if (input->GetMousePos().y >= (int) sObj.position.y && input->GetMousePos().y <= (int) sObj.position.y + sObj.sprite.GetHeight()) {
				if (input->GetMouseButton(1)) {
					sObj.sprite.SetFrame(1);
					sObj.sprite.Draw(screen, (int) sObj.position.x, (int) sObj.position.y);
				}
				if (input->GetMouseButtonUp(1)) {
					return true;
				}
			}
		}
		return false;
	}
};