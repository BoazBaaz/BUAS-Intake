#include "game.h"
#include <iostream>
#include <string> 

namespace Tmpl8 {
	// Variables (Main Scene)
	Sprite sbSprite(new Surface("assets/start_button.tga"), 2);
	StaticObject startButton(sbSprite, vec2(300, 412));


	// Variables (Game Scene)
	Sprite ballSprite(new Surface("assets/ball.png"), 1);
	DynamicObject player(ballSprite, vec2(175, 462), vec2(1, -50), vec2(100, -100));

	// On start
	void Game::Init() {
	}

	// On close
	void Game::Shutdown() {
	}

	// Every frame
	void Game::Tick(float _deltaTime) {
		screen->Clear(0);

		switch (curScene) {
			case main:
				if (Button(startButton)) {
					ChangeScene(game);
				}
				break;
			case game:
				VelocityVerlet(player, _deltaTime);
				player.m_Sprite.Draw(screen, player.m_Position.x, player.m_Position.y);
				break;
			default:
				break;
		}
	}


	// Fuctions
	void Game::VelocityVerlet(DynamicObject &dObj, float dt) {
		dt /= 100;
		dObj.m_Position += (dObj.m_Velocity * dt) + ((dObj.m_Acceleration / 2) * (dt * dt));
		dObj.m_Velocity += dObj.m_Acceleration * dt;
		dObj.m_Acceleration.x *= 0.9;
		dObj.m_Acceleration.y = gravity * 0.9;
	}


	void Game::ChangeScene(Scene newScene) {
		curScene = newScene; screen->Clear(0);
	}

	bool Game::Button(StaticObject sObj) {
		sObj.sprite.SetFrame(0);
		sObj.sprite.Draw(screen, (int)sObj.position.x, (int)sObj.position.y);
		if (mouse.position.x >= (int)sObj.position.x && mouse.position.x <= (int)sObj.position.x + sObj.sprite.GetWidth()) {
			if (mouse.position.y >= (int)sObj.position.y && mouse.position.y <= (int)sObj.position.y + sObj.sprite.GetHeight()) {
				if (GetMouseButton(1)) {
					sObj.sprite.SetFrame(1);
					sObj.sprite.Draw(screen, (int)sObj.position.x, (int)sObj.position.y);
				}
				if (GetMouseButtonUp(1)) {
					return true;
				}
			}
		}
		return false;
	}
};