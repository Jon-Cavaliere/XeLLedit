#include "Input.h"

JC::Input::Input()
    : m_Keyboard(), m_QuitFlag(false), m_Event(), m_Mouse(), m_Scroll(0)
{
    m_MouseButtons[0] = m_MouseButtons[1] = m_MouseButtons[2] = 0;
}

void JC::Input::update()
{
    // Reset Mouse Scroll State
    m_Scroll = 0;

	while (SDL_PollEvent(&m_Event))
	{
		m_Keyboard.Update();
		if (m_Event.type == SDL_QUIT)
			m_QuitFlag = true;
        else if (m_Event.type == SDL_MOUSEWHEEL)
        {
            if (m_Event.wheel.y > 0) // scroll up
            {
                m_Scroll = 1;
            }
            else if (m_Event.wheel.y < 0) // scroll down
            {
                m_Scroll = -1;
            }

        }
        else if (m_Event.type == SDL_MOUSEBUTTONDOWN)
        {
            // ... handle mouse clicks ...
        }
        else if (m_Event.type == SDL_MOUSEMOTION)
        {
            m_Mouse[0] = m_Event.motion.x;
            m_Mouse[1] = m_Event.motion.y;
        }
        if (m_Event.type == SDL_MOUSEBUTTONDOWN)
        {
            switch (m_Event.button.button)
            {
            case(SDL_BUTTON_LEFT):
                m_MouseButtons[0] = 1;
                break;
            case(SDL_BUTTON_MIDDLE):
                m_MouseButtons[1] = 1;
                break;
            case(SDL_BUTTON_RIGHT):
                m_MouseButtons[2] = 1;
                break;
            }
        }
        else if (m_Event.type == SDL_MOUSEBUTTONUP)
        {
            switch (m_Event.button.button)
            {
            case(SDL_BUTTON_LEFT):
                m_MouseButtons[0] = 0;
                break;
            case(SDL_BUTTON_MIDDLE):
                m_MouseButtons[1] = 0;
                break;
            case(SDL_BUTTON_RIGHT):
                m_MouseButtons[2] = 0;
                break;
            }
        }
        
	}
}

bool JC::Input::isKeyDown(uint8_t key)
{
	return m_Keyboard.isKeyDown(key);
}

bool JC::Input::isMouseDown(uint8_t button)
{
    if(button > 2)
        return false;
    return m_MouseButtons[button];
}

uint32_t JC::Input::getKeyDownTime(uint8_t key)
{
    return m_Keyboard.getDownTime(key);
}

int8_t JC::Input::getScrolling()
{
    return m_Scroll;
}

int32_t* JC::Input::getCursorPos()
{
    return m_Mouse;
}