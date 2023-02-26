#include "input.h"

namespace Tmpl8 {
	Input::Input() {
		for (int i = 0; i < m_MaxKeys; i++) {
			m_Keyboard.keys[i] = InputState::Released;
		}
		for (int i = 0; i < m_MaxButtons; i++) {
			m_Mouse.buttons[i] = InputState::Released;
		}
	}

	int Input::InputStateMod(int a_CurState, int a_NewState) {
		if (!a_CurState && !a_NewState) { a_NewState = (int)InputState::Released; }
		if (a_CurState && a_NewState) { a_NewState = (int)InputState::Pressed; }
		if (a_CurState && !a_NewState) { a_NewState = (int)InputState::Up; }
		if (!a_CurState && a_NewState) { a_NewState = (int)InputState::Down; }
		return a_NewState;
	}

	void Input::UpdateInputState() {
		for (int i = 0; i < m_MaxKeys; i++) {
			if (m_Keyboard.keys[i] == InputState::Down) { m_Keyboard.keys[i] = InputState::Pressed; }
			else if (m_Keyboard.keys[i] == InputState::Up) { m_Keyboard.keys[i] = InputState::Released; }
		}
		for (int i = 0; i < m_MaxButtons; i++) {
			if (m_Mouse.buttons[i] == InputState::Down) { m_Mouse.buttons[i] = InputState::Pressed; }
			else if (m_Mouse.buttons[i] == InputState::Up) { m_Mouse.buttons[i] = InputState::Released; }
		}
	}

	void Input::MouseMove(int x, int y) {
		m_Mouse.position.x = (float)x, m_Mouse.position.y = (float)y;
		m_Mouse.pixel = (x + ScreenWidth * y);
	}

	void Input::KeyInput(int key, int state) { 
		m_Keyboard.keys[key] = (InputState)InputStateMod((int) m_Keyboard.keys[key], state);
	}

	void Input::MouseInput(int button, int state) { 
		m_Mouse.buttons[button] = (InputState)InputStateMod((int) m_Mouse.buttons[button], state);
	}
}