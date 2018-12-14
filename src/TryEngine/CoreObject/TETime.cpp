#include "CoreObject/TETime.hpp"

TETime::TETime(): m_tickTime(m_clock.restart())
{}

void TETime::Tick()
{
    m_tickTime = m_clock.restart();
}