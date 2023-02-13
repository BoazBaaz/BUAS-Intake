#include "input.h"

namespace Tmpl8 {
	int Input::InputStateMod(int curState, int newState) {
		if (!curState && !newState) { newState = (int)InputState::Released; }
		if (curState && newState) { newState = (int)InputState::Pressed; }
		if (curState && !newState) { newState = (int)InputState::Up; }
		if (!curState && newState) { newState = (int)InputState::Down; }
		return newState;
	}

	void Input::UpdateInputState() {
		for (int i = 0; i < MAX_KEYS; i++) {
			if (keyboard.keys[i] == InputState::Down) { keyboard.keys[i] = InputState::Pressed; }
			else if (keyboard.keys[i] == InputState::Up) { keyboard.keys[i] = InputState::Released; }
		}
		for (int i = 0; i < MAX_BUTTONS; i++) {
			if (mouse.buttons[i] == InputState::Down) { mouse.buttons[i] = InputState::Pressed; }
			else if (mouse.buttons[i] == InputState::Up) { mouse.buttons[i] = InputState::Released; }
		}
	}

	void Input::KeyInput(int key, int state) { 
		keyboard.keys[key] = (InputState)InputStateMod((int)keyboard.keys[key], state); 
	}

	void Input::MouseInput(int button, int state) { 
		mouse.buttons[button] = (InputState)InputStateMod((int)mouse.buttons[button], state);
	}
	void Input::MouseMove(int x, int y) {
		mouse.position.x = (float)x, mouse.position.y = (float)y;
		mouse.pixel = (x + ScreenWidth * y);
	}
}