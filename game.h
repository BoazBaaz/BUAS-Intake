#pragma once

#include "surface.h"
#include "SDL_Scancode.h"
#include <iostream>
#include <string>
#include <chrono>

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
			struct Position {
				double x, y;
				int pixel;
			} position;
		};
	};

	struct Vector2 { 
		// Variables
		double x, y; 

		// Constructor
		Vector2(double x = 0, double y = 0) : x(x), y(y) {};

		// Operators
		Vector2 operator =(const Vector2& v) { x = v.x; y = v.y; return *this; };
		Vector2 operator +(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
		Vector2 operator -(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
		Vector2 operator +=(const Vector2& v) { x += v.x; y += v.y; return *this; }
		Vector2 operator -=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
	};
	struct StaticObject {
		// Variables
		Sprite& sprite;
		Vector2 position;

		// Constructor
		StaticObject(Sprite& sprite, Vector2 position) : 
			sprite(sprite), position(position) 
		{};
	};
	struct DynamicObject {
		// Variables
		Sprite& sprite;
		Vector2 position;
		Vector2 velocity;
		Vector2 acceleration;
		double mass;

		// Constructor
		DynamicObject(Sprite& sprite, Vector2 position, Vector2 velocity, Vector2 acceleration, double mass) : 
			position(position), sprite(sprite), velocity(velocity), acceleration(acceleration), mass(mass)
		{};
	};

	class Surface; 
	class Game {
	public:
		void SetTarget(Surface* surface) { screen = surface; }
		void Init();
		void Shutdown();
		void Tick(float _deltaTime);
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
		void KeyInput(int key, int state) { keyboard.keys[key] = InputStateMod((int)keyboard.keys[key], state); }
		void MouseInput(int button, int state) { mouse.buttons[button] = InputStateMod((int)mouse.buttons[button], state); }
		void MouseMove(int x, int y) {
			mouse.position.x = x, mouse.position.y = y;
			mouse.position.pixel = ( x + screen->GetWidth() * y );
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
		Scene curScene;

		const double gravity = 9.81;

		Input::Keyboard keyboard;
		Input::Mouse mouse;
		
		// Physics
		void VelocityVerlet(DynamicObject &dObj, float dt);

		// Scene
		void ChangeScene(Scene newScene);

		// Input
		bool Button(StaticObject sObj);
		bool GetKey(int key) { return (keyboard.keys[key] == Input::InputState::Pressed) ? true : false; };
		bool GetKeyUp(int key) { return ( keyboard.keys[key] == Input::InputState::Up ) ? true : false; };
		bool GetKeyDown(int key) { return ( keyboard.keys[key] == Input::InputState::Down ) ? true : false; };
		bool GetMouseButton(int button) { return (mouse.buttons[button] == Input::InputState::Pressed) ? true : false; };
		bool GetMouseButtonUp(int button) { return ( mouse.buttons[button] == Input::InputState::Up ) ? true : false; };
		bool GetMouseButtonDown(int button) { return (mouse.buttons[button] == Input::InputState::Down) ? true : false; };
	};
};