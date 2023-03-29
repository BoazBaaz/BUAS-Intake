#include "game.h"

namespace Tmpl8 {
	// >> Variables (Main Scene) << //
	// Background
	Sprite m_TitleBGSprite(new Surface("assets/title_background.png"), 1);

	//Buttons
	Sprite m_StartButtonSprite(new Surface("assets/start_button.png"), 2);
	UI m_StartButton(m_StartButtonSprite, vec2(300, 412));

	// >> Variables (Game Scene) << //
	// Background
	Sprite m_GameBGSprite(new Surface("assets/cloud_background.png"), 1);
	vec2 m_GameBGPosition(0, 0);
	float m_GameBGSlideSpeed = 100.0f;

	// Player
	Sprite m_PlayerSprite(new Surface("assets/ball.png"), 1);
	Player m_Player(m_PlayerSprite, vec2(375, 231), vec2(0, 0), 40);

	// Platforms
	Sprite m_PlatformSprite(new Surface("assets/platform200x40.png"), 1);
	Platform m_Platform0(m_PlatformSprite, vec2(300, 282), vec2(0, 1), 40);
	Platform m_Platform1(m_PlatformSprite, vec2(100, 82), vec2(0, 1), 40);
	Platform m_Platform2(m_PlatformSprite, vec2(500, -118), vec2(0, 1), 40);
	Platform m_Platforms[3] = {m_Platform0, m_Platform1, m_Platform2};

	// On start
	void Game::Init() {
		// load the highscore
		std::ifstream saveFile("saveData/highscore.save");
		saveFile >> m_HighScore;
		saveFile.close();
	}

	// On close
	void Game::Shutdown() {
		// save the highscore
		std::ofstream saveFile("saveData/highscore.save");
		saveFile << m_HighScore;
		saveFile.close();
	}

	// Every frame
	void Game::Tick(float dt) {
		screen->Clear(0);

		switch (m_Scene) {
			case Scene::Title:
				// draw the background
				m_TitleBGSprite.Draw(screen, 0, 0);

				// update the button then check if the button was pressed
				m_StartButton.Update(screen, input);
				if (m_StartButton.Pressed()) {
					ChangeScene(Scene::Game);
				}
				break;
			case Scene::Game:
				// if the background is at the end of the screen set the position on the x axis to 0, slide it to the right
				if (m_GameBGPosition.x >= ScreenWidth) {
					m_GameBGPosition = 0;
				} else {
					m_GameBGPosition.x += m_GameBGSlideSpeed * dt;
				}
				std::cout << m_Score << std::endl;
				// draw the background at the position and to the left screenwidth value of the position
				m_GameBGSprite.Draw(screen, (int) m_GameBGPosition.x, (int) m_GameBGPosition.y);
				m_GameBGSprite.Draw(screen, (int) m_GameBGPosition.x - ScreenWidth, (int) m_GameBGPosition.y);

				// update and draw all the object in the game
				m_Player.Update(this, screen, input, dt);
				for (Platform& platform : m_Platforms) {
					platform.Update(screen, dt);
					m_Player.PlatformCollision(this, platform);
				}
				break;
			case Scene::Gameover:
				break;
			default:
				break;
		}
	}

	void Game::Gameover() {
		if (m_HighScore < m_Score) {
			m_HighScore = m_Score;
		}

		ChangeScene(Scene::Gameover);
	}
};