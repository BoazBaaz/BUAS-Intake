#pragma once

#include "game.h"

namespace Tmpl8 {
	class Input {
		enum class InputState { Released, Pressed, Up, Down };
	public:
		// constructor / destructor
		Input();
		~Input() = default;
		// member data access
		vec2 GetMousePos() { return m_Mouse.position; }
		int GetMousePosPixel() { return m_Mouse.pixel; }
		bool GetKey(int a_Key) { return (m_Keyboard.keys[a_Key] == InputState::Pressed) ? true : false; }
		bool GetKeyUp(int a_Key) { return (m_Keyboard.keys[a_Key] == InputState::Up) ? true : false; }
		bool GetKeyDown(int a_Key) { return (m_Keyboard.keys[a_Key] == InputState::Down) ? true : false; }
		bool GetMouseButton(int a_Button) { return (m_Mouse.buttons[a_Button] == InputState::Pressed) ? true : false; }
		bool GetMouseButtonUp(int a_Button) { return (m_Mouse.buttons[a_Button] == InputState::Up) ? true : false; }
		bool GetMouseButtonDown(int a_Button) { return (m_Mouse.buttons[a_Button] == InputState::Down) ? true : false; }
		// Special operations
		void UpdateInputState();
		void MouseMove(int a_X, int a_Y);
		void KeyInput(int a_Key, int a_State);
		void MouseInput(int a_Button, int a_State);
		int InputStateMod(int a_CurState, int a_NewState);
	private:
		// static attributes
		const static int m_MaxKeys = 512;
		const static int m_MaxButtons = 6;
		// attributes
		struct Keyboard {
			InputState keys[m_MaxKeys] = {};
		} m_Keyboard;
		struct Mouse {
			InputState buttons[m_MaxButtons] = {};
			vec2 position;
			int pixel = 0;
		} m_Mouse;
	};
}