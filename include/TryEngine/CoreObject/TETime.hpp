#ifndef TERO_TIME_HPP
#define TERO_TIME_HPP

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

class TETime
{
    sf::Clock m_clock;
    sf::Time m_tickTime;

    public:
    TETime();
    void Tick();
    sf::Time Raw() const { return m_tickTime; }
    sf::Clock Clock() const { return m_clock; }
    float operator()() {
        return m_tickTime.asSeconds();
    }
    float Elapsed() const { return m_clock.getElapsedTime().asSeconds(); };
};

#endif