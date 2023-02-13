#pragma once

#include "surface.h"
#include "template.h"
#include <iostream>

namespace Tmpl8 {
	class Input {
	private:
		enum class InputState { Released, Pressed, Up, Down };

		const static int MAX_KEYS = 512;
		const static int MAX_BUTTONS = 6;

		int InputStateMod(int curState, int newState);

	public:
		struct Keyboard {
			InputState keys[MAX_KEYS];
		} keyboard;
		struct Mouse {
			InputState buttons[MAX_BUTTONS];
			vec2 position;
			int pixel;
		} mouse;

		void UpdateInputState();
		void KeyInput(int key, int state);
		void MouseInput(int button, int state);
		void MouseMove(int x, int y);

		bool GetKey(int key) { return (keyboard.keys[key] == InputState::Pressed) ? true : false; };
		bool GetKeyUp(int key) { return (keyboard.keys[key] == InputState::Up) ? true : false; };
		bool GetKeyDown(int key) { return (keyboard.keys[key] == InputState::Down) ? true : false; };
		bool GetMouseButton(int button) { return (mouse.buttons[button] == InputState::Pressed) ? true : false; };
		bool GetMouseButtonUp(int button) { return (mouse.buttons[button] == InputState::Up) ? true : false; };
		bool GetMouseButtonDown(int button) { return (mouse.buttons[button] == InputState::Down) ? true : false; };
	};
}