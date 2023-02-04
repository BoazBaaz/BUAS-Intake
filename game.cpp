#include "game.h"
#include <iostream>
#include <string> 

namespace Tmpl8 {
	// Variables (Main Scene)
	Sprite sbSprite(new Surface("assets/start_button.tga"), 2);
	StaticObject startButton(sbSprite, vec2(300, 412));


	// Variables (Game Scene)
	Sprite ballSprite(new Surface("assets/ball.png"), 1);
	DynamicObject player(ballSprite, vec2(375, 462), vec2(4, -20), vec2(0, 0));

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
				if (GetKey(SDL_SCANCODE_SPACE)) {
					player.vel.y = 10;
					if (groundHit) {
						player.vel = {0, 0};
						std::string bstr = std::to_string(boost);
						screen->Print(&bstr[0], player.pos.x, player.pos.y - 10, 0xffffff);
						boost = (boost < 3) ? (boost + dt * 3) : 3;
					}
				}

				// Update velocity and reset boost
				if (GetKeyUp(SDL_SCANCODE_SPACE)) {
					player.vel = (mouse.position -player.pos) / 100;
					player.vel *= boost;
					boost = 0;
				}
				std::cout << "New Vel: " << player.vel.x << ", " << player.vel.y << std::endl;
				std::cout << "Boost: " << boost << std::endl;

				Physics(player, dt);
				player.sprite.Draw(screen, player.pos.x, player.pos.y);
				break;
			default:
				break;
		}
	}

	void Game::Physics(DynamicObject& p, float dt) {
		// Apply gravity to the velocity 
		if (!groundHit) {
			p.vel.y += gravity * dt;
		}

		// Update the velocity
		p.vel += p.acc * dt;
		p.vel *= deceleration;

		// Update position
		p.pos += p.vel;

		// Collision detection
		if (p.pos.x < 0 || p.pos.x + p.sprite.GetWidth() > ScreenWidth) {
			p.pos.x = std::max((float) 0, std::min(p.pos.x, (float) ScreenWidth - p.sprite.GetWidth()));
			p.vel.x = -p.vel.x;
		}
		if (p.pos.y < 0 || p.pos.y + p.sprite.GetHeight() > ScreenHeight) {
			p.pos.y = std::max((float) 0, std::min(p.pos.y, (float) ScreenHeight - p.sprite.GetHeight()));
			p.vel.y = -p.vel.y;
		}

		// Ground check
		groundHit = (p.pos.y == (ScreenHeight - p.sprite.GetHeight())) ? true : false;

		std::cout << "Pos: x " << player.pos.x << ", y " << player.pos.y << std::endl;
		std::cout << "Vel: x " << player.vel.x << ", y " << player.vel.y << std::endl;
		std::cout << "Acc: x " << player.acc.x << ", y " << player.acc.y << std::endl;
		std::cout << "Ground Hit: " << groundHit << std::endl;

		// Reset the acceleration for the next caculation
		p.acc = {0, 0};
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