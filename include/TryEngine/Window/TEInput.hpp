#ifndef TERO_INPUTHANDLER_HPP
#define TERO_INPUTHANDLER_HPP

#include <SFML/Graphics/RenderWindow.hpp>

class TEInput
{
    public:
    TEInput();

    void Handle(sf::Event& event);
    
    bool ReceivedCloseEvent() const { return m_queryingClose; }

    private:
    bool m_queryingClose;
};

#endif