#include "game.h"

namespace Tmpl8 {
	// Variables (Main Scene)
	Sprite startButtonSprite(new Surface("assets/start_button.png"), 2);
	GameObject startButton(startButtonSprite, vec2(300, 412));

	// Variables (Game Scene)
	Sprite backgroundSprite(new Surface("assets/cloud_background.png"), 1);
	vec2 bgPosition(0, 0);
	float bgSlideSpeed = 100.0f;

	Sprite playerSprite(new Surface("assets/ball.png"), 1);
	Player player(playerSprite, vec2(375, 460), vec2(0, 0), 40);

	// Platform Objectpool :)
	Sprite platformSprite(new Surface("assets/platform200x40.png"), 1);
	GameObject platform0(platformSprite, vec2(100, -100), vec2(0, 1), 40, true, true);
	GameObject platform1(platformSprite, vec2(250, -300), vec2(0, 1), 40, true, true);
	GameObject platform2(platformSprite, vec2(400, -400), vec2(0, 1), 40, true, true);
	GameObject platform3(platformSprite, vec2(550, -200), vec2(0, 1), 40, true, true);

	GameObject platforms[4] = {platform0, platform1, platform2, platform3};

	// On start
	void Game::Init() {
	}

	// On close
	void Game::Shutdown() {
	}

	// Every frame
	void Game::Tick(float dt) {
		screen->Clear(0);

		switch (m_Scene) {
			case Scene::Main:
				if (Button(startButton)) {
					ChangeScene(Scene::Game);
				}
				break;
			case Scene::Game:
				// add bg_position and draw the background
				if (bgPosition.x >= ScreenWidth) {
					bgPosition = 0;
				} else {
					bgPosition.x += bgSlideSpeed * dt;
				}
				backgroundSprite.Draw(screen, (int)bgPosition.x, (int)bgPosition.y);
				backgroundSprite.Draw(screen, (int)bgPosition.x - ScreenWidth, (int)bgPosition.y);

				// update and draw all the object in the game
				player.Update(this, screen, input, dt);
				for (GameObject& platform : platforms) {
					platform.Update(screen, dt);
					player.PlayerCollision(platform);
				}
				break;
			default:
				break;
		}
	}

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