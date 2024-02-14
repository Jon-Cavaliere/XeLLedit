#pragma once
#include <SDL.h>
#include "Keyboard.h"
#include "Window.h"
// Create a keystate struct, then a keyboard class to store state


namespace JC
{
	class Input
	{
	public:
		Input();

		void update(Window* win);

		bool isKeyDown(uint8_t key);
		bool isMouseDown(uint8_t button);

		// Returns amount of a time a key has been held down ( 0 = not pressed )
		uint32_t getKeyDownTime(uint8_t key);

		inline bool shouldQuit() { return m_QuitFlag; }

		int8_t getScrolling();
		int32_t* getCursorPos();
	
	private:
		Keyboard m_Keyboard;
		SDL_Event m_Event;
		bool m_QuitFlag;
		// Mouse scroll, -1 = down, 0 = not scrolling, 1 = up
		int8_t m_Scroll;

		// Cursor Location {x, y}
		int32_t m_Mouse[2]; 

		// Mouse button states (left = 0, center = 1, right = 2)
		uint8_t m_MouseButtons[3];

	};
}