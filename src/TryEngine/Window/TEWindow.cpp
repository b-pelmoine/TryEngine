#include "TryEngine/Window/TEWindow.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

TEWindow::TEWindow(TEWindowOptions&& options) : 
m_options(options), m_active(false), m_input(std::make_shared<TEInput>())
{
    ApplyConfig();
}

TEWindow::TEWindow(std::shared_ptr<sf::RenderWindow> window) : 
m_window(window), m_active(true), m_input(std::make_shared<TEInput>())
{
    SaveCurrentConfig();
}

void TEWindow::Config(const TEWindowOptions& options)
{
    m_options = options;
    ApplyConfig();
}

void TEWindow::SaveCurrentConfig()
{
    m_options.size      = m_window->getSize();
    m_options.settings  = m_window->getSettings();
}

void TEWindow::ApplyConfig()
{
    sf::VideoMode mode = (m_options.style == sf::Style::Fullscreen)
    ? m_options.fullScreenMode : sf::VideoMode(m_options.size.x, m_options.size.y);

    if(!m_window)
    {
        m_window = std::make_shared<sf::RenderWindow>(
            mode,
            m_options.name,
            m_options.style,
            m_options.settings
            );
        m_active = true;
    }
    else
    {
        m_window->create(
            mode,
            m_options.name,
            m_options.style,
            m_options.settings
        );
    }

    m_window->setPosition(m_options.position);
}

void TEWindow::HandleEvents()
{
    sf::Event event;
    while (m_window->pollEvent(event)) {
        m_input->Handle(event);
    }
}

void TEWindow::Draw()
{
    m_window->clear(sf::Color::Black);
    //Draw
    m_window->display();
}