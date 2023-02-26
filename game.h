#pragma once

#include "surface.h"
#include "template.h"
#include "input.h"
#include "SDL_Scancode.h"
#include <iostream>
#include <string>

namespace Tmpl8 {
	class Surface;
	class Input;
	class Game {
	public:
		struct StaticObject {
			Sprite& sprite;
			vec2 position;

			StaticObject(Sprite& sprite, vec2 position) :
				sprite(sprite), position(position) {
			};
		};
		struct DynamicObject {
			Sprite& sprite;
			vec2 pos, vel, acc;

			DynamicObject(Sprite& sprite, vec2 position, vec2 velocity, vec2 acceleration) :
				sprite(sprite), pos(position), vel(velocity), acc(acceleration) {
			};
		};

		void SetTarget(Surface* surface, Input* controls) { screen = surface; input = controls; }
		void Init();
		void Shutdown();
		void Tick(float dt);

	private:
		Surface* screen;
		Input* input;

		enum Scene { main, game } curScene = main;

		const double gravity = 9.81;
		const double deceleration = 0.995;

		bool Button(StaticObject sObj);
		void Physics(DynamicObject& p, float dt);
		void Collision(DynamicObject& p);
		void ChangeScene(Scene newScene) { curScene = newScene; screen->Clear(0); };
	};
};