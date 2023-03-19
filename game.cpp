#include "game.h"

namespace Tmpl8 {
	// Variables (Main Scene)
	Sprite s_StartButton(new Surface("assets/start_button.tga"), 2);
	GameObject startButton(s_StartButton, vec2(300, 412));

	// Variables (Game Scene)
	Sprite s_Player(new Surface("assets/ball.png"), 1);
	Player player(s_Player, vec2(375, 460), vec2(8, -20), 40);

	Sprite s_Platform(new Surface("assets/balk.png"), 1);
	GameObject platform0(s_Platform, vec2(100, 200));
	GameObject platform1(s_Platform, vec2(500, 200));

	GameObject gameObjects[2] = {platform0, platform1};

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
				
				for (GameObject& gameObject : gameObjects) {
					gameObject.Update(screen);
				}
				player.Update(screen, input, dt);
				player.PlayerCollision(platform1);
				player.BouncePhysics(this, dt);
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