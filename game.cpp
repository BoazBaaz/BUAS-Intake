#include "game.h"

namespace Tmpl8 {
	// Variables (Main Scene)
	Sprite s_StartButton(new Surface("assets/start_button.tga"), 2);
	GameObject startButton(s_StartButton, vec2(300, 412));

	// Variables (Game Scene)
	Sprite s_Player(new Surface("assets/ball.png"), 1);
	Player player(s_Player, vec2(375, 462), vec2(4, -20), 20);

	Sprite s_Platform0(new Surface("assets/balk.png"), 1);
	GameObject platform0(s_Platform0, vec2(500, 200));
	Sprite s_Platform1(new Surface("assets/balk.png"), 1);
	GameObject platform1(s_Platform1, vec2(100, 200));

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
				
				player.Update(this, screen, input, dt);
				for (GameObject& gameObject : gameObjects) {
					gameObject.Update(this, screen, dt);
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