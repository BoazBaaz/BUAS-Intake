#pragma once

#include "surface.h"
#include "template.h"
#include "SDL_Scancode.h"
#include <iostream>
#include <string>

namespace Tmpl8 {
	namespace Input {
		enum class InputState { Released, Pressed, Up, Down };

		constexpr int MAX_KEYS = 512;
		constexpr int MAX_BUTTONS = 6;

		class Keyboard {
		public:
			InputState keys[MAX_KEYS];
		};

		class Mouse {
		public:
			InputState buttons[MAX_BUTTONS];
			vec2 position;
			int pixel;
		};
	};

	struct StaticObject {
		// Variables
		Sprite& sprite;
		vec2 position;

		// Constructor
		StaticObject(Sprite& sprite, vec2 position) :
			sprite(sprite), position(position) {
		};
	};
	struct DynamicObject {
		// Variables
		Sprite& sprite;
		vec2 pos, vel, acc;

		// Constructor
		DynamicObject(Sprite& sprite, vec2 position, vec2 velocity, vec2 acceleration) :
			sprite(sprite), pos(position), vel(velocity), acc(acceleration) {
		};
	};

	class Surface;
	class Game {
	public:
		void SetTarget(Surface* surface) { screen = surface; }
		void Init();
		void Shutdown();
		void Tick(float dt);
		void UpdateInputState() {
			for (int i = 0; i < Input::MAX_KEYS; i++) {
				if (keyboard.keys[i] == Input::InputState::Down) { keyboard.keys[i] = Input::InputState::Pressed; }
				else if (keyboard.keys[i] == Input::InputState::Up) { keyboard.keys[i] = Input::InputState::Released; }
			}
			for (int i = 0; i < Input::MAX_BUTTONS; i++) {
				if (mouse.buttons[i] == Input::InputState::Down) { mouse.buttons[i] = Input::InputState::Pressed; }
				else if (mouse.buttons[i] == Input::InputState::Up) { mouse.buttons[i] = Input::InputState::Released; }
			}
		}
		void KeyInput(int key, int state) { keyboard.keys[key] = InputStateMod((int) keyboard.keys[key], state); }
		void MouseInput(int button, int state) { mouse.buttons[button] = InputStateMod((int) mouse.buttons[button], state); }
		void MouseMove(int x, int y) {
			mouse.position.x = x, mouse.position.y = y;
			mouse.pixel = (x + screen->GetWidth() * y);
		}
		Input::InputState InputStateMod(int curState, int newState) {
			Input::InputState state;
			if (!curState && !newState) { state = Input::InputState::Released; }
			if (curState && newState) { state = Input::InputState::Pressed; }
			if (curState && !newState) { state = Input::InputState::Up; }
			if (!curState && newState) { state = Input::InputState::Down; }
			return state;
		}

	private:
		enum Scene { main, game };

		Surface* screen;
		Scene curScene = Scene::main;

		const double gravity = 9.81;
		const double deceleration = 0.995;

		Input::Keyboard keyboard;
		Input::Mouse mouse;

		// Physics
		void Physics(DynamicObject& p, float dt);

		// Scene
		void ChangeScene(Scene newScene) { curScene = newScene; screen->Clear(0); };

		// Input
		bool Button(StaticObject sObj);
		bool GetKey(int key) { return (keyboard.keys[key] == Input::InputState::Pressed) ? true : false; };
		bool GetKeyUp(int key) { return (keyboard.keys[key] == Input::InputState::Up) ? true : false; };
		bool GetKeyDown(int key) { return (keyboard.keys[key] == Input::InputState::Down) ? true : false; };
		bool GetMouseButton(int button) { return (mouse.buttons[button] == Input::InputState::Pressed) ? true : false; };
		bool GetMouseButtonUp(int button) { return (mouse.buttons[button] == Input::InputState::Up) ? true : false; };
		bool GetMouseButtonDown(int button) { return (mouse.buttons[button] == Input::InputState::Down) ? true : false; };
	};
};