#include "game.h"

namespace Tmpl8 {
	// Backgrounds
	Sprite bgTitleSprite(new Surface("assets/title_background.png"), 1);
	Sprite bgLoseSprite(new Surface("assets/lose_background.png"), 1);
	Sprite bgGameSprite(new Surface("assets/cloud_background.png"), 1);
	vec2 bgGamePosition(0, 0);
	float bgGameSlideSpeed = 100.0f;

	// Player
	Sprite playerSprite(new Surface("assets/ball.png"), 1);
	Player player(playerSprite, {375, 231}, {0, 0}, 40);

	// Platforms
	Sprite platformSprite(new Surface("assets/platform200x40.png"), 1);
	Platform platform0(platformSprite, {300, 282}, {0, 1}, 40);
	Platform platform1(platformSprite, {100, 82}, {0, 1}, 40);
	Platform platform2(platformSprite, {500, -118}, {0, 1}, 40);
	Platform platforms[3] = {platform0, platform1, platform2};

	//Buttons
	Sprite startButtonSprite(new Surface("assets/start_button.png"), 3);
	Sprite retryButtonSprite(new Surface("assets/retry_button.png"), 3);
	Sprite quitButtonSprite(new Surface("assets/quit_button.png"), 3);
	UI startButton(startButtonSprite, vec2(300, 280));
	UI quitButton(quitButtonSprite, vec2(300, 380));
	UI retryButton(retryButtonSprite, vec2(164, 360));
	UI returnButton(quitButtonSprite, vec2(410, 360));

	// On start
	void Game::Init() {
		// load the highscore
		std::ifstream saveFile("saveData/highscore.save");
		saveFile >> highScore;
		saveFile.close();
	}

	// On close
	void Game::Shutdown() {
		// save the highscore
		std::ofstream saveFile("saveData/highscore.save");
		saveFile << highScore;
		saveFile.close();
	}

	// Every frame
	int Game::Tick(float dt) {
		screen->Clear(0);
		std::cout << curScore << std::endl;

		switch (curScene) {
			case Scene::Title:
				// draw the background
				bgTitleSprite.Draw(screen, 0, 0);

				// update the start button then check if the button was pressed
				startButton.Update(screen, input);
				if (startButton.Released()) {
					StartGame();
				}

				// update the quit button then check if the button was pressed
				quitButton.Update(screen, input);
				if (quitButton.Released()) {
					return 1;
				}
				break;
			case Scene::Game:
				// if the background is at the end of the screen set the position on the x axis to 0, slide it to the right
				if (bgGamePosition.x >= ScreenWidth) {
					bgGamePosition = 0;
				} else {
					bgGamePosition.x += bgGameSlideSpeed * dt;
				}

				// draw the background at the position and to the left screenwidth value of the position
				bgGameSprite.Draw(screen, (int) bgGamePosition.x, (int) bgGamePosition.y);
				bgGameSprite.Draw(screen, (int) bgGamePosition.x - ScreenWidth, (int) bgGamePosition.y);

				// update and draw all the object in the game
				player.Update(this, screen, input, dt);
				for (Platform& platform : platforms) {
					platform.Update(screen, dt);
					player.PlatformCollision(this, platform);
				}

				screen->Print("Score: ", 0, 0, 0);
				//screen->Print((char*) curScore, 0, 0, 0); //TODO: display score
				break;
			case Scene::Gameover:
				bgLoseSprite.Draw(screen, 0, 0);

				// update the retry button then check if the button was pressed
				retryButton.Update(screen, input);
				if (retryButton.Released()) {
					StartGame();
				}

				// update the quit button then check if the button was pressed
				returnButton.Update(screen, input);
				if (returnButton.Released()) {
					ChangeScene(Scene::Title);
				}
				break;
			default:
				break;
		}
		return 0;
	}

	void Game::StartGame() {
		curScore = 0;

		// reset the player and platforms transform and speed
		player.Reset();
		for (Platform& platform : platforms) {
			platform.Reset();
		}

		ChangeScene(Scene::Game);
	}

	void Game::GameOver() {
		if (highScore < curScore) {
			highScore = curScore;
		}

		ChangeScene(Scene::Gameover);
	}
};