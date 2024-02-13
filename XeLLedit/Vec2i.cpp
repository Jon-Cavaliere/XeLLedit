#include "Vec2i.h"
#include <stdexcept>

JC::Vec2i::Vec2i(int32_t x, int32_t y)
	: m_Data{ x, y }
{}

JC::Vec2i::Vec2i(const Vec2i& v)
{
	m_Data[0] = v.getX();
	m_Data[1] = v.getY();
}

JC::Vec2i::Vec2i()
	: m_Data { 0, 0 }
{}

void JC::Vec2i::setX(int32_t x)
{
	m_Data[0] = x;
}

void JC::Vec2i::setY(int32_t y)
{
	m_Data[1] = y;
}

void JC::Vec2i::set(int32_t x, int32_t y)
{
	setX(x);
	setY(y);
}

int32_t JC::Vec2i::getX() const
{
	return m_Data[0];
}

int32_t JC::Vec2i::getY() const
{
	return m_Data[1];
}

int32_t& JC::Vec2i::operator[](int8_t index)
{
	if (index <= 1)
		return m_Data[index];
	else
	{
		throw std::runtime_error("Error: Vec2i element index out of bounds\n");
	}
}
