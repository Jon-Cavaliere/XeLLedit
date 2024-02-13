#include "Vec2f.h"
#include <stdexcept>
#include <cmath>
JC::Vec2f::Vec2f(float x, float y)
    : m_Data{ x, y }
{
}

JC::Vec2f::Vec2f(const Vec2f& v)
{
    m_Data[0] = v.getX();
    m_Data[1] = v.getY();
}

JC::Vec2f::Vec2f(const Vec2i& v)
    : Vec2f((float)v.getX(), (float)v.getY())
{
}

JC::Vec2f::Vec2f(int32_t x, int32_t y)
    : Vec2f((float)x, (float)y)
{
}

JC::Vec2f::Vec2f()
    : m_Data{ 0.0f, 0.0f }
{
}

void JC::Vec2f::setX(float x)
{
    m_Data[0] = x;
}

void JC::Vec2f::setY(float y)
{
    m_Data[1] = y;
}

float JC::Vec2f::getX() const
{
    return m_Data[0];
}

float JC::Vec2f::getY() const
{
    return m_Data[1];
}

void JC::Vec2f::normalize()
{
    // Get magnitude
    float mag = std::sqrt(m_Data[0] * m_Data[0] + m_Data[1] * m_Data[1]);
    // Divide each component by magnitude
    if (mag != 0)
    {
        m_Data[0] /= mag;
        m_Data[1] /= mag;
    }
}

float& JC::Vec2f::operator[](int8_t index)
{
    if (index <= 1)
        return m_Data[index];
    else
    {
        throw std::runtime_error("Error: Vec2i element index out of bounds\n");
    }
}

void JC::Vec2f::operator*=(float scalar)
{
    m_Data[0] *= scalar;
    m_Data[1] *= scalar;

}
