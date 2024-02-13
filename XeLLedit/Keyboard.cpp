#include "Keyboard.h"

JC::Keyboard::Keyboard()
{
	p_SDLKeyStates = const_cast <uint8_t*>(SDL_GetKeyboardState(NULL));

	for (uint8_t i = 0; i < 255; i++)
	{
		KeyState k = { false, 0 };
		m_KeyStates.insert({ i, k});
	}
}

void JC::Keyboard::Update()
{
	for (uint8_t i = 0; i < 255; i++)
	{
		bool isDown = (p_SDLKeyStates[i]);
		// if the new state is pressed, and the old state was up
		// then set down time to current time
		if (m_KeyStates[i].pressed != isDown && isDown == true)
		{
			m_KeyStates[i].downTime = SDL_GetTicks();
		}
		if (m_KeyStates[i].pressed != isDown && isDown == false)
		{
			m_KeyStates[i].downTime = 0; // reset down time to 0
		}
		
		m_KeyStates[i].pressed = isDown;

	}
}

bool JC::Keyboard::isKeyDown(uint8_t key)
{
	return m_KeyStates[key].pressed;	
}

uint32_t JC::Keyboard::getDownTime(uint8_t key)
{
	if (!isKeyDown(key))
		return 0;
	else
		return SDL_GetTicks() - m_KeyStates[key].downTime;
}
