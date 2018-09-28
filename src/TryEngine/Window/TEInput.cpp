#include "Window/TEInput.hpp"
#include <SFML/Window/Event.hpp>

TEInput::TEInput()
{}

void TEInput::Handle(sf::Event& event)
{
    if(event.type == sf::Event::EventType::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        m_queryingClose = true;
}
