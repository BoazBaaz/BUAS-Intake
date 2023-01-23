#include "game.h"
#include <iostream>
#include <string> 

namespace Tmpl8 {
	// Variables (Main Scene)
	Sprite sbSprite(new Surface("assets/start_button.tga"), 2);
	StaticObject startButton(sbSprite, Vector2(300, 412));


	// Variables (Game Scene)
	Sprite ballSprite(new Surface("assets/ball.png"), 1);
	DynamicObject player(ballSprite, Vector2(375, 462), Vector2(0, 0), Vector2(0, 0), 1);

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
				
				break;
			default:
				break;
		}
	}


	// Fuctions
	void VelocityVerlet(DynamicObject &dObj, float dt) {
		dObj.position += dt * ( dObj.velocity + dt * dObj.acceleration / 2 );
		dObj.velocity += dt * dObj.acceleration;
	}


	void Game::ChangeScene(Scene newScene) {
		curScene = newScene; screen->Clear(0);
	}

	bool Game::Button(StaticObject sObj) {
		sObj.sprite.SetFrame(0);
		sObj.sprite.Draw(screen, sObj.position.x, sObj.position.y);
		if (mouse.position.x >= sObj.position.x && mouse.position.x <= sObj.position.x + sObj.sprite.GetWidth()) {
			if (mouse.position.y >= sObj.position.y && mouse.position.y <= sObj.position.y + sObj.sprite.GetHeight()) {
				if (GetMouseButton(1)) {
					sObj.sprite.SetFrame(1);
					sObj.sprite.Draw(screen, sObj.position.x, sObj.position.y);
				}
				if (GetMouseButtonUp(1)) {
					return true;
				}
			}
		}
		return false;
	}
};