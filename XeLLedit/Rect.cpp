#include "Rect.h"

JC::Rect::Rect(int32_t x, int32_t y, int32_t w, int32_t h)
	: m_Pos{x, y}, m_Size{w, h}, m_Color(JC::Color(255, 255, 255))
{
}

JC::Rect::Rect(int32_t x, int32_t y, int32_t w, int32_t h, JC::Color color)
	: m_Pos{ x, y }, m_Size{ w, h }, m_Color(color)
{
}

void JC::Rect::set(int32_t x, int32_t y, int32_t w, int32_t h)
{
	set(x, y);
	setSize(w, h);
}

void JC::Rect::set(int32_t x, int32_t y)
{
	m_Pos[0] = x;
	m_Pos[1] = y;
}

void JC::Rect::setSize(int32_t w, int32_t h)
{
	m_Size[0] = w;
	m_Size[1] = h;
}

void JC::Rect::setColor(JC::Color color)
{
	m_Color = color;
}

JC::Vec2i& JC::Rect::getPos()
{
	return m_Pos;
}

JC::Vec2i& JC::Rect::getSize()
{
	return m_Size;
}

int32_t JC::Rect::getCenterX()
{
	return m_Pos[0] + m_Size[0] / 2;
}

int32_t JC::Rect::getCenterY()
{
	return m_Pos[1] + m_Size[1] / 2;
}

int32_t JC::Rect::getX()
{
	return m_Pos[0];
}

int32_t JC::Rect::getY()
{
	return m_Pos[1];
}

void JC::Rect::setX(int32_t x)
{
	m_Pos[0] = x;
}

void JC::Rect::setY(int32_t y)
{
	m_Pos[1] = y;
}

void JC::Rect::getCenter(Vec2i& center)
{
	center.setX(getCenterX());
	center.setY(getCenterY());
}

SDL_Rect JC::Rect::toSDLRect()
{
	return { m_Pos[0], m_Pos[1], m_Size[0], m_Size[1] };
}

void JC::Rect::Render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	SDL_Rect renderBox = toSDLRect();
	SDL_RenderFillRect(renderer, &renderBox);
}

// Axis-aligned bounding-box
bool JC::Rect::checkCollision(Rect& a, Rect& b)
{
	int xa = a.getX();
	int ya = a.getY();
	int wa = a.getSize()[0];
	int ha = a.getSize()[1];

	int xb = b.getX();
	int yb = b.getY();
	int wb = b.getSize()[0];
	int hb = b.getSize()[1];

	if (xa + wa > xb && xa < xb + wb && ya + ha > yb && ya < yb + hb)
		return true;
	return false;
}
