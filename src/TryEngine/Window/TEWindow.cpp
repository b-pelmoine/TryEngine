#include "TryEngine/Window/TEWindow.hpp"

#include <TryEngine.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

TEWindow::TEWindow(TEWindowOptions&& options) : 
m_options(options), m_texture(std::make_shared<sf::RenderTexture>()), m_input(std::make_shared<TEInput>()), m_active(false), bUsePostProcess(false)
{
    for(size_t i=0; i < static_cast<size_t>(Layer::_Count); ++i)
    {
        m_drawables[static_cast<Layer>(i)] = std::vector<std::weak_ptr<sf::Drawable>>();
    }
    ApplyConfig();
}

TEWindow::TEWindow(std::shared_ptr<sf::RenderWindow> window) : 
m_window(window), m_texture(std::make_shared<sf::RenderTexture>()), m_input(std::make_shared<TEInput>()), m_active(true), bUsePostProcess(true)
{
    for(size_t i=0; i < static_cast<size_t>(Layer::_Count); ++i)
    {
        m_drawables[static_cast<Layer>(i)] = std::vector<std::weak_ptr<sf::Drawable>>();
    }
    SaveCurrentConfig();
    InitializeRenderTexture();
}

void TEWindow::ConfigurePostProcess(const std::string& shaderID)
{
    bUsePostProcess = !shaderID.empty();
    if(bUsePostProcess)
        TE.Resources().lock()->GetShader(shaderID, m_postProcessShader);
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
    m_active = true;

    InitializeRenderTexture();

    m_window->setPosition(m_options.position);
}

void TEWindow::InitializeRenderTexture()
{
    if(m_options.style == sf::Style::Fullscreen)
    {
        auto size = m_window->getSize();
        m_texture->create(size.x, size.y);
    }
    else
    {
        m_texture->create(m_options.size.x, m_options.size.y);
    }
}

void TEWindow::HandleEvents()
{
    sf::Event event;
    while (m_window->pollEvent(event)) {
        m_input->Handle(event);
    }
    if(m_input->ReceivedCloseEvent()) 
    {
        m_active = false;
    }
}

void TEWindow::PostUpdate()
{
    m_input->PostUpdate();
}

void TEWindow::Draw()
{
    m_texture->clear(sf::Color::Blue);
    for(const auto& layer: m_drawables)
    {
        for(const auto& drawable: layer.second)
        {
            m_texture->draw(*(drawable.lock()));
        }
    }
    m_texture->display();
    m_window->clear(sf::Color::Black);
    sf::Sprite processedPicture(m_texture->getTexture());
    if(bUsePostProcess)
    {
        m_window->draw(processedPicture, &m_postProcessShader->Get());
    }
    else
    {
        m_window->draw(processedPicture);
    }
}

void TEWindow::Display()
{
    m_window->display();
}

void TEWindow::AddDrawable(std::shared_ptr<sf::Drawable> drawable, const Layer& layer)
{
    m_drawables[layer].push_back(drawable);
}

void TEWindow::RemoveDrawable(std::shared_ptr<sf::Drawable> drawable, const Layer& layer)
{
    auto& vector = m_drawables[layer];
    vector.erase(std::remove_if(vector.begin(), vector.end(), 
    [drawable](std::weak_ptr<sf::Drawable> other) { 
        return drawable == other.lock(); 
        }), vector.end());
}