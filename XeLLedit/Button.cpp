#include "Button.h"

Button::Button(int32_t x, int32_t y, int32_t w, int32_t h) :
	m_Location(x,y), m_Dimensions(w,h), m_Hovered(false), m_Sheet()
{

}

void Button::init(const char* spriteFile, SDL_Renderer* renderer)
{
	m_Sheet.loadFromFile(spriteFile, false, renderer);
}

void Button::update(JC::Input* in)
{
	JC::Vec2i mouse;
	//TODO copy constructor for Vec2i... because this isn't optimal
	mouse.set(in->getCursorPos()[0], in->getCursorPos()[1]);
	
	// Check if mouse is hovering button and set the hover flag accordingly
	if (mouse[0] >= m_Location[0] && mouse[0] <= m_Location[0] + m_Dimensions[0] && mouse[1] >= m_Location[1] && mouse[1] <= m_Location[1] + m_Dimensions[1])
		m_Hovered = true;
	else
		m_Hovered = false;
}

void Button::render(SDL_Renderer* renderer)
{
	if (m_Hovered)
	{
		SDL_Rect clip = { m_Dimensions[0], 0, m_Dimensions[0], m_Dimensions[1] };
		m_Sheet.renderSize(m_Location[0], m_Location[1], m_Dimensions[0], m_Dimensions[1], &clip);
	}
	else
	{
		SDL_Rect clip = { 0, 0, m_Dimensions[0], m_Dimensions[1] };
		m_Sheet.renderSize(m_Location[0], m_Location[1], m_Dimensions[0], m_Dimensions[1], &clip);
	}
	
}

bool Button::isHover()
{
	return m_Hovered;
}

int32_t Button::getX()
{
	return m_Location[0];
}

int32_t Button::getY()
{
	return m_Location[1];
}

int32_t Button::getW()
{
	return m_Dimensions[0];
}

int32_t Button::getH()
{
	return m_Dimensions[1];
}
