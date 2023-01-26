#include "game.h"
#include <iostream>
#include <string> 

namespace Tmpl8 {
	// Variables (Main Scene)
	Sprite sbSprite(new Surface("assets/start_button.tga"), 2);
	StaticObject startButton(sbSprite, vec2(300, 412));


	// Variables (Game Scene)
	Sprite ballSprite(new Surface("assets/ball.png"), 1);
	DynamicObject player(ballSprite, vec2(375, 462), vec2(40, -100), vec2(0, 0));

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
				Physics(player, dt);
				player.sprite.Draw(screen, player.pos.x, player.pos.y);
				if (GetKey(SDL_SCANCODE_W)) {
					player.acc.y -= 10 * dt;
				}
				if (GetKey(SDL_SCANCODE_A)) {
					player.acc.x -= 10 * dt;
				}
				if (GetKey(SDL_SCANCODE_D)) {
					player.acc.x += 10 * dt;
				}

				std::cout << "Pos: x " << player.pos.x << ", y " << player.pos.y << std::endl;
				std::cout << "Vel: x " << player.vel.x << ", y " << player.vel.y << std::endl;
				std::cout << "Acc: x " << player.acc.x << ", y " << player.acc.y << std::endl;
				break;
			default:
				break;
		}
	}

	void Game::Physics(DynamicObject& p, float dt) {
		// Update position
		p.pos += p.vel * dt + (p.acc / 2) * (dt * dt);

		// Update velocity
		// 1000 = 1000 + 10
		// 1010
		p.vel += p.acc * dt;
		// 1010 = 1010 * 0.9
		// 909
		p.vel *= deceleration;

		// Update acceleration
		p.acc.x *= deceleration;
		p.acc.y = gravity * deceleration;


		// Collision detection
		if (p.pos.x < 0 || p.pos.x + p.sprite.GetWidth() > ScreenWidth) {
			p.pos.x = std::max((float)0, std::min(p.pos.x, (float)ScreenWidth));
			// 900.98 = -900.98
			p.vel.x = -p.vel.x;
			p.acc.x = -p.acc.x;
		}
		if (p.pos.y < 0 || p.pos.y + p.sprite.GetHeight() > ScreenHeight) {
			p.pos.y = std::max((float) 0, std::min(p.pos.y, (float) ScreenHeight));
			p.vel.y = -p.vel.y;
			p.acc.y = -p.acc.y;
		}
	}


	bool Game::Button(StaticObject sObj) {
		sObj.sprite.SetFrame(0);
		sObj.sprite.Draw(screen, (int) sObj.position.x, (int) sObj.position.y);
		if (mouse.position.x >= (int) sObj.position.x && mouse.position.x <= (int) sObj.position.x + sObj.sprite.GetWidth()) {
			if (mouse.position.y >= (int) sObj.position.y && mouse.position.y <= (int) sObj.position.y + sObj.sprite.GetHeight()) {
				if (GetMouseButton(1)) {
					sObj.sprite.SetFrame(1);
					sObj.sprite.Draw(screen, (int) sObj.position.x, (int) sObj.position.y);
				}
				if (GetMouseButtonUp(1)) {
					return true;
				}
			}
		}
		return false;
	}
};