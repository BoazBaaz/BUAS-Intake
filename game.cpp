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
	Platform platforms[3] = {
		Platform(platformSprite, {300, 282}, {0, 1}), 
		Platform(platformSprite, {100, 82}, {0, 1}), 
		Platform(platformSprite, {500, -118}, {0, 1})
	};

	// Buttons
	Sprite startButtonSprite(new Surface("assets/start_button.png"), 3);
	Sprite retryButtonSprite(new Surface("assets/retry_button.png"), 3);
	Sprite quitButtonSprite(new Surface("assets/quit_button.png"), 3);
	UI startButton(startButtonSprite, vec2(300, 280));
	UI quitButton(quitButtonSprite, vec2(300, 380));
	UI retryButton(retryButtonSprite, vec2(164, 360));
	UI returnButton(quitButtonSprite, vec2(410, 360));

	// Score
	Sprite scoreSprite(new Surface("assets/score/score_text.png"), 1);
	Sprite highScoreSprite(new Surface("assets/score/highscore_text.png"), 1);
	Sprite digit0Sprite(new Surface("assets/score/digit_0.png"), 1);
	Sprite digit1Sprite(new Surface("assets/score/digit_1.png"), 1);
	Sprite digit2Sprite(new Surface("assets/score/digit_2.png"), 1);
	Sprite digit3Sprite(new Surface("assets/score/digit_3.png"), 1);
	Sprite digit4Sprite(new Surface("assets/score/digit_4.png"), 1);
	Sprite digit5Sprite(new Surface("assets/score/digit_5.png"), 1);
	Sprite digit6Sprite(new Surface("assets/score/digit_6.png"), 1);
	Sprite digit7Sprite(new Surface("assets/score/digit_7.png"), 1);
	Sprite digit8Sprite(new Surface("assets/score/digit_8.png"), 1);
	Sprite digit9Sprite(new Surface("assets/score/digit_9.png"), 1);
	int digitWidth = digit0Sprite.GetWidth();
	vec2 scorePosition(8, 2);
	vec2 highScorePosition(336, 236);

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

		switch (curScene) {
			case Scene::Title:
				// draw the background
				bgTitleSprite.Draw(screen, 0, 0);

				// draw highscore
				DrawScore(highScoreSprite, highScorePosition, highScore);

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
				for (Platform& platform : platforms) {
					player.PlatformCollision(this, platform);
					platform.Update(this, screen, player, dt);
				}
				player.Update(this, screen, input, dt);

				DrawScore(scoreSprite, scorePosition, curScore);
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

	void Game::DrawScore(Sprite& sprite, vec2 position, int score) {
		sprite.Draw(screen, (int)position.x, (int)position.y);
		vec2 digitPosition(position.x + sprite.GetWidth(), position.y);
		DrawDigits(digitPosition, score);
	}

	void Game::DrawDigits(vec2 position, int value) {
		// get the score in individual digits
		int digit1 = (value / 10) % 10;
		int digit2 = value % 10;

		// if there are more then 2 digits in the digits int return
		if (value > 99) {
			digit1 = 9;
			digit2 = 9;
		}

		// create an array of all the digits
		int digits[2] = {digit1, digit2};

		// draw the digits of curScore
		for (int i = 0; i < 2; i++) {
			switch (digits[i])
			{
				case 0:
					digit0Sprite.Draw(screen, (int)position.x + (digitWidth * i), (int)position.y);
					break;
				case 1:
					digit1Sprite.Draw(screen, (int)position.x + (digitWidth * i), (int)position.y);
					break;
				case 2:
					digit2Sprite.Draw(screen, (int)position.x + (digitWidth * i), (int)position.y);
					break;
				case 3:
					digit3Sprite.Draw(screen, (int)position.x + (digitWidth * i), (int)position.y);
					break;
				case 4:
					digit4Sprite.Draw(screen, (int)position.x + (digitWidth * i), (int)position.y);
					break;
				case 5:
					digit5Sprite.Draw(screen, (int)position.x + (digitWidth * i), (int)position.y);
					break;
				case 6:
					digit6Sprite.Draw(screen, (int)position.x + (digitWidth * i), (int)position.y);
					break;
				case 7:
					digit7Sprite.Draw(screen, (int)position.x + (digitWidth * i), (int)position.y);
					break;
				case 8:
					digit8Sprite.Draw(screen, (int)position.x + (digitWidth * i), (int)position.y);
					break;
				case 9:
					digit9Sprite.Draw(screen, (int)position.x + (digitWidth * i), (int)position.y);
					break;
				default:
					break;
			}
		}
	};

	void Game::StartGame() {
		curScore = 0;
		platformSpeed = 40.0f;

		// reset the player and platforms transform
		player.Reset();
		for (Platform& platform : platforms) {
			platform.Reset();
			platform.SetPlatformPassed(false);
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