#include "RateRegulator.h"

JC::RateRegulator* JC::RateRegulator::p_Instance = nullptr;

JC::RateRegulator* JC::RateRegulator::getInstance()
{
    if (p_Instance == nullptr)
    {
        p_Instance = new RateRegulator();
        return p_Instance;
    }
    else
    {
        return p_Instance;
    }
}

void JC::RateRegulator::begin()
{
    // Physics loop
    m_CurrentTime = SDL_GetTicks();

    // Calculate dT (in seconds)
    m_DeltaTime = (m_CurrentTime - m_LastTime) / 1000.0f;
}

void JC::RateRegulator::end()
{
    uint32_t secs = m_DeltaTime * 1000.f;
    if (secs < 1000 / m_FrameCap)
        SDL_Delay(1000 / m_FrameCap - secs);
    // Set updated time
    m_LastTime = m_CurrentTime;



}

float JC::RateRegulator::dt()
{
    return m_DeltaTime;
}

JC::RateRegulator::RateRegulator()
    :m_CurrentTime(0), m_DeltaTime(0.0f), m_FrameCap(60.0f), m_LastTime(0), m_CapFPS(true)
{
}
