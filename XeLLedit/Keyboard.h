#pragma once
#include <map>
#include <SDL.h>
/**
* Keyboard class works with SDL_ScanCodes
*/
namespace JC
{
	struct KeyState
	{
		bool pressed = false;
		uint32_t downTime;
	};

	class Keyboard
	{
	public:
		Keyboard();
		void Update();
		// Returns whether or not the key is currently down
		bool isKeyDown(uint8_t key);
		// Returns amount of time a key has been down, 0 = not down
		uint32_t getDownTime(uint8_t key);
	private:
		// Scancode, State
		std::map<uint8_t, KeyState> m_KeyStates;
		uint8_t* p_SDLKeyStates;
	};
}

