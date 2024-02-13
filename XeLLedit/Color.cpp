#include "Color.h"
#include <stdexcept>

// Initialize our statics
JC::Color JC::Color::RED = JC::Color(255, 0, 0);
JC::Color JC::Color::GREEN = JC::Color(0, 255, 0);
JC::Color JC::Color::BLUE = JC::Color(0, 0, 255);
JC::Color JC::Color::WHITE = JC::Color(255, 255, 255);
JC::Color JC::Color::BLACK = JC::Color(0, 0, 0);

JC::Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    m_Channels[0] = r;
    m_Channels[1] = g;
    m_Channels[2] = b;
    m_Channels[3] = a;
}

JC::Color::Color(uint8_t r, uint8_t g, uint8_t b)
    : Color(r, g, b, 255)
{
    
}

JC::Color::Color(const JC::Color& c)
{
    m_Channels[0] = c.read(0);
    m_Channels[1] = c.read(1);
    m_Channels[2] = c.read(2);
    m_Channels[3] = c.read(3);
}

void JC::Color::set(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    m_Channels[0] = r;
    m_Channels[1] = g;
    m_Channels[2] = b;
    m_Channels[3] = a;
}

uint8_t& JC::Color::get(uint8_t index)
{
    if(index >= 0 && index < 4)
        return m_Channels[index];
    else
    {
        throw std::runtime_error("Error: Color channel index out of bounds\n");
    }
}

const uint8_t JC::Color::read(uint8_t index) const
{
    if (index >= 0 && index < 4)
        return m_Channels[index];
    else
    {
        throw std::runtime_error("Error: Color channel index out of bounds\n");
    }
}

JC::Color JC::Color::operator=(const Color& c)
{
    m_Channels[0] = c.read(0);
    m_Channels[1] = c.read(1);
    m_Channels[2] = c.read(2);
    m_Channels[3] = c.read(3);

    return *this;
}

uint8_t& JC::Color::operator[](uint8_t index)
{
    if (index < 4)
        return m_Channels[index];
    else
        throw std::runtime_error("Error: Color channel index out of bounds\n");
}

SDL_Color JC::Color::toSDL()
{
    return { m_Channels[0], m_Channels[1], m_Channels[2], m_Channels[3] };
}
